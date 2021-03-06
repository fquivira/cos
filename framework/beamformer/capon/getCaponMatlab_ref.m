%GETCAPON Implementation of the Minimum Variance / Capon filter for linear
%         arrays. Includes FB averaging, time averaging, and the use of
%         subspaces. 
%
% [imAmplitude imPower] = getCapon(dataCube, indsI, indsJ, regCoef, L, nTimeAverage, V, doForwardBackwardAveraging, verbose)
%
% dataCube     : A range x beams x array-elements image cube
% indsI, indsJ : Indices of pixels that are to be processed; indsI==0 / indsJ==0 means all ranges / beams
% regCoef      : Regularization coefficient for diagonal loading
% L            : Length of subarrays
% nTimeAverage : Includes +- this number of time lags to produce 'R' matrix
% V            : The columns span an orthogonal subspace; if V is empty ([]) than the full space is used (no subspacing)
% doForwardBackwardAveraging : Whether to do forward-backward averaging
% usePinv      : {Default: false} Whether or not to use the 'pinv' command when inverting the matrix
%
% Note I: If using a subspace matrix, V, enabling FB averaging gives a
% substantial increase in the computational load.  This is because the
% reduction of the dimensionality must happen at a much later stage in the
% code. 
%
% Note II: This version assumes a=ones(L,1) in w = Ri*a/(a'*Ri*a), i.e., we
% search for the amplitude/power in the direction perpedicular to the
% linear array.
%
% Note III: Matrix inversion is done like this: Ri = inv(R + regCoef/L*trace(R)*I); 
% or using pinv if so chosen.
%
% Last modified:
% 2009.08.25 - Are C. Jensen {Created the function}
% 2009.08.27 - Are C. Jensen {Robustified indsI in light of nTimeAverage use}
% 2009.09.09 - Are C. Jensen {By popular request, added the factor 1/L in the diagonal loading}
% 2011.06.24 - Are C. Jensen {Added the 'usePinv' parameter}
% 2011.06.29 - Are C. Jensen {Reverse index order when creating the 'd' matrix (reshuffling data) to gain speed}
function [imAmplitude imPower] = getCaponMatlab_ref(dataCube, indsI, indsJ, regCoef, L, nTimeAverage, V, doForwardBackwardAveraging, verbose, usePinv)

tic;
if nargin < 10
  usePinv = false;
end

[N M K] = size(dataCube);
if indsI<=0, indsI = 1:N;, end;
if indsJ<=0, indsJ = 1:M;, end;

% Skip pixels that cannot be calculated due to time averaging
indsI = indsI( indsI > nTimeAverage );
indsI = indsI( indsI < (N-nTimeAverage+1) );

a = ones(L,1);
n = nTimeAverage;
imAmplitude = zeros(N,M);
imPower = zeros(N,M);
I = eye(L);
useSubspace = ~isempty(V);

if useSubspace
  nSubspaceDims = length(V(1,:));
  if verbose, fprintf('Capon algorithm "subspaced" down to %d dims.\n', nSubspaceDims);, end;
  I = eye( nSubspaceDims );
  a = V'*a;  % The column of ones (what we seek the 'magnitude' of) represented in the subspace
end

%disp('before for()')
for i=indsI
  %disp('new x')
  for j=indsJ
    %disp('new y')
    ar = squeeze(dataCube(i-n:i+n, j, :));  % Array responses plus-minus n time steps
    if n==0, ar = transpose(ar);, end;

    % Place the array responses in one (K-L+1)*(2n+1) x L matrix:
    ar = transpose(ar);
    d = zeros(L, (K-L)*(2*n+1));
    for ii=1:K-L+1
      d(:, ((ii-1)*(2*n+1)+1):((ii)*(2*n+1))) = ar(ii:ii+L-1, :);
    end
    d = d';
 
    % If a subspace matrix V is given and we're _not_ using FB averaging, we
    % can use V to reduce the dimensionality of the data now:
    if (useSubspace) && (~doForwardBackwardAveraging)
      d = d*V;
    end;
    
    R = d'*d / (K-L+1) / (2*n+1);   % R contains an estimate of the covariance matrix

    % Store the sum of the current-time outputs in 'g_singleSnapshot':
    g_singleSnapshot = sum(d(n+1:(2*n+1):end, :))' / (K-L+1);

    if doForwardBackwardAveraging
      R = 0.5*(R + rot90(conj(R), 2));
    end

    % If a subspace matrix V is given and we _are_ using FB averaging, we
    % have to wait until now to go to the reduced space:
    if (useSubspace) && (doForwardBackwardAveraging)
      R = V'*R*V;
      g_singleSnapshot = V'*g_singleSnapshot;
    end;

%    if usePinv
%      Ri = pinv(R + regCoef/L*trace(R)*I);
%    else
      Ri = inv(R + regCoef/L*trace(R)*I);
%    end

    w = Ri*a / (a'*Ri*a);
    imAmplitude(i,j) = w'*g_singleSnapshot;   % Note: A bit ad-hoc maybe, but uses only the current time-snapshot to calculate the output/'alpha' value
    imPower(i,j) = abs(w'*R*w);
    
  end
  
  if verbose
    percent = round( 100*(i-min(indsI))/(max(indsI)-min(indsI)) );
    if mod(i,5)==0, fprintf(' %d%%', percent);, end;
    if mod(i,100)==0, fprintf('\n');, end;
  end
end

tt = toc;
fn = fopen('timer_matlab.txt','a');
fprintf(fn, '%f\n', tt);
