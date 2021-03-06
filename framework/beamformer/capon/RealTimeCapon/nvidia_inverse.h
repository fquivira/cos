/*
 * Copyright 1993-2011 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO LICENSEE:
 *
 * This source code and/or documentation ("Licensed Deliverables") are
 * subject to NVIDIA intellectual property rights under U.S. and
 * international Copyright laws.
 *
 * These Licensed Deliverables contained herein is PROPRIETARY and
 * CONFIDENTIAL to NVIDIA and is being provided under the terms and
 * conditions of a form of NVIDIA software license agreement by and
 * between NVIDIA and Licensee ("License Agreement") or electronically
 * accepted by Licensee.  Notwithstanding any terms or conditions to
 * the contrary in the License Agreement, reproduction or disclosure
 * of the Licensed Deliverables to any third party without the express
 * written consent of NVIDIA is prohibited.
 *
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, NVIDIA MAKES NO REPRESENTATION ABOUT THE
 * SUITABILITY OF THESE LICENSED DELIVERABLES FOR ANY PURPOSE.  IT IS
 * PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND.
 * NVIDIA DISCLAIMS ALL WARRANTIES WITH REGARD TO THESE LICENSED
 * DELIVERABLES, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY
 * SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THESE LICENSED DELIVERABLES.
 *
 * U.S. Government End Users.  These Licensed Deliverables are a
 * "commercial item" as that term is defined at 48 C.F.R. 2.101 (OCT
 * 1995), consisting of "commercial computer software" and "commercial
 * computer software documentation" as such terms are used in 48
 * C.F.R. 12.212 (SEPT 1995) and is provided to the U.S. Government
 * only as a commercial end item.  Consistent with 48 C.F.R.12.212 and
 * 48 C.F.R. 227.7202-1 through 227.7202-4 (JUNE 1995), all
 * U.S. Government End Users acquire the Licensed Deliverables with
 * only those rights set forth herein.
 *
 * Any use of the Licensed Deliverables in individual and commercial
 * software must include, in the user documentation and internal
 * comments to the code, the above Disclaimer and U.S. Government End
 * Users Notice.
 */

#if !defined(INVERSE_H_)
#define INVERSE_H_
#include "cuComplex.h"

#ifdef __cplusplus
extern "C" {
#endif

/* dmatinv_batch() inverts one or many square, non-singular matrices of double-
   precision elements. Partial pivoting is employed in the inversion process 
   for increased numerical stability.

   A     pointer to an array of the double-precision matrices to be inverted, 
         where each matrix is stored in column-major order
   Ainv  pointer to an array of the double-precision matrices which receive
         the inverses of the corresponding matrices pointed to by A, where 
         each matrix is stored in column-major order
   n     number of rows and columns of the matrices in the arrays pointed to 
         by A and Ainv. n must be greater than, or equal to 2. On sm_13 GPUs,
         n must be less than, or equal to, 44. On sm_2x GPUs, n must be less 
         than, or equal to, 77.
   batch the number of matrices to be inverted. It must be greater than zero.

   Returns:

    0    operation completed successfully
   -1    n is out of bounds, batch is out of bounds
   -2    a CUDA error occured
*/
int dmatinv_batch(double *A, double *Ainv, int n, int batch);

/* zmatinv_batch() inverts one or many square, non-singular matrices of double-
   complex elements. Partial pivoting is employed in the inversion process for
   increased numerical stability.

   A     pointer to an array of the double-complex matrices to be inverted, 
         where each matrix is stored in column-major order
   Ainv  pointer to an array of the double-complex matrices which receive
         the inverses of the corresponding matrices pointed to by A, where 
         each matrix is stored in column-major order
   n     number of rows and columns of the matrices in the arrays pointed to 
         by A and Ainv. n must be greater than, or equal to, 2. On sm_13 GPUs,
         n must be less than, or equal to, 31. On sm_2x GPUs, n must be less 
         than, or equal to, 55.
   batch the number of matrices to be inverted. It must be greater than zero.

   Returns:

    0    operation completed successfully
   -1    n is out of bounds, batch is out of bounds
   -2    a CUDA error occured
*/
int zmatinv_batch(cuDoubleComplex *A, cuDoubleComplex *Ainv, int n, int batch);

// single precision versions
int fmatinv_batch (float *A, float *Ainv, int n, int batch);
int zfmatinv_batch (cuComplex *A, cuComplex *Ainv, int n, int batch);

#ifdef __cplusplus
}
#endif

#endif /* INVERSE_H_ */
