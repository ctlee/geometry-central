#pragma once

#include "geometrycentral/utilities/utilities.h"

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/StdVector>

#include <complex>
#include <iostream>

// === Various helper functions and sanity checks which are useful for linear algebra code


namespace geometrycentral {

// Convenience typedefs

// Nicer name for dynamic matrix
template <typename T>
using Vector = Eigen::Matrix<T, Eigen::Dynamic, 1>;

// Nicer name for sparse matrix
template <typename T>
using SparseMatrix = Eigen::SparseMatrix<T>;

// Nicer name for dense matrix
template <typename T>
using DenseMatrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

// ==== Simple utilities

template <typename T>
SparseMatrix<T> identityMatrix(size_t N);

// Shift the the diagonal of a matrix by a constant offset
template <typename T>
void shiftDiagonal(SparseMatrix<T>& m, T shiftAmount = 1e-4);

template <typename T>
SparseMatrix<T> verticalStack(const std::vector<SparseMatrix<T>, Eigen::aligned_allocator<SparseMatrix<T>>>& mats);

template <typename T>
SparseMatrix<T> horizontalStack(const std::vector<SparseMatrix<T>, Eigen::aligned_allocator<SparseMatrix<T>>>& mats);

// Blow up an NxM complex system to a 2N x2M real system.
SparseMatrix<double> complexToReal(const SparseMatrix<std::complex<double>>& m);
Vector<double> complexToReal(const Vector<std::complex<double>>& v);

// ==== Sanity checks


// Verify that a matrix has finite entries, error out if not
template <typename T>
void checkFinite(const SparseMatrix<T>& m);

template <typename T, int R, int C>
void checkFinite(const Eigen::Matrix<T, R, C>& m);

// Verify that a sparse matrix is symmetric, error out if not.
template <typename T>
void checkSymmetric(const SparseMatrix<T>& m, double absoluteEPS = -1.);

// Verify that a sparse matrix is hermitian, error out if not.  For real matrices, coincides with checkSymmetric
template <typename T>
void checkHermitian(const SparseMatrix<T>& m, double absoluteEPS = -1.);


// ==== Permutations and blocking

// Build a permutation matrix
// template <typename T>
// SparseMatrix<T> buildPermutationMatrix(const Vector<size_t>& p);

// Apply a permutation to the rows and columns of a matrix
// template <typename T>
// void permute(const SparseMatrix<T>& m, const Vector<size_t>& p);


// Block-decompose a square sparse matrix with interleaved index sets A and B
template <typename T>
struct BlockDecompositionResult {
  // The index of each element of A (resp. B) in the original system
  Vector<size_t> origIndsA;
  Vector<size_t> origIndsB;

  // Index of each orignal element in the new system  (either in the A system or B)
  Vector<size_t> newInds;
  Vector<bool> isA;

  // The four "block" matrices
  SparseMatrix<T> AA;
  SparseMatrix<T> AB;
  SparseMatrix<T> BA;
  SparseMatrix<T> BB;
};
template <typename T>
BlockDecompositionResult<T> blockDecomposeSquare(const SparseMatrix<T>& m, const Vector<bool>& Aset,
                                                 bool buildBuildBside = true);

// Apply a decomposition to a vector
template <typename T>
void decomposeVector(BlockDecompositionResult<T>& decomp, const Vector<T>& vec, Vector<T>& vecAOut, Vector<T>& vecBOut);
template <typename T>
Vector<T> reassembleVector(BlockDecompositionResult<T>& decomp, const Vector<T>& vecA, const Vector<T>& vecB);

#include "geometrycentral/numerical/linear_algebra_utilities.ipp"

} // namespace geometrycentral
