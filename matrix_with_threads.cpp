#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>
using namespace std;

// Define mutex for synchronization
mutex mtx_guard;

// Define matrix type
typedef vector<vector<int>> matrix;

/**
 * Performs matrix multiplication for a specific range of rows.
 *
 * @param mat1 first matrix
 * @param mat2 second matrix
 * @param result the resultant matrix of mat1 x mat2 in the range
 * @param start the row to start from
 * @param end the row to end on
 */
void multiplyrow(const matrix &mat1, const matrix &mat2, matrix &result,
                 int start, int end) {
  int col2 = mat2[0].size();
  int shared_length = mat2.size();

  for (int row = start; row < end; ++row) {
    for (int i = 0; i < col2; ++i) {
      for (int j = 0; j < shared_length; ++j) {
        mtx_guard.lock();
        result[row][i] += mat1[row][j] * mat2[j][i];
        mtx_guard.unlock();
      }
    }
  }
}

/**
 * Performs matrix multiplication using threads
 *
 * @param mat1 first matrix
 * @param mat2 second matrix
 * @param result the resultant matrix of mat1 x mat2 in the range
 * @param numThreads the number of threads
 */
void multiplyMatrices(const matrix &mat1, const matrix &mat2, matrix &result,
                      int numThreads) {
  int row1 = mat1.size();
  int col1 = mat1[0].size();
  int col2 = mat2[0].size();
  int row2 = mat2.size();

  cout << "matrix 1: " << row1 << " x " << col1 << endl;
  cout << "matrix 2: " << row2 << " x " << col2 << endl << endl;

  // check if the number of col in the 1st matrix = number of row in the
  // second matrix
  if (col1 != row2) {
    throw runtime_error(
        "matrix multiplication not possible, incompatible dimensions.");
  }

  // Initialize result matrix with correct size
  // dim = row1 x col2
  // set default values to 0
  result.resize(row1, vector<int>(col2, 0));

  // set maximum thread count to prevent computation waste
  if (numThreads > row1) {
    numThreads = row1;
  }

  // calculate the number of rows for each thread
  int chunk = row1 / numThreads;
  int remainder = row1 % numThreads;

  // Vector to hold thread objects
  vector<thread> threads;

  // Start and end row for each thread
  int start, end = 0;

  // Create threads
  for (int i = 0; i < numThreads; ++i) {
    end = start + chunk + (i < remainder ? 1 : 0);
    threads.emplace_back(multiplyrow, ref(mat1), ref(mat2), ref(result), start,
                         end);
    start = end;
  }

  // Join threads
  for (auto &thread : threads) {
    thread.join();
  }
}

/**
 * displays a matrix
 *
 * @param mat a matrix to display
 */
void displayMatrix(const matrix &mat) {
  for (const auto &row : mat) {
    for (int element : row) {
      cout << setfill(' ') << setw(4) << element << " ";
    }
    cout << endl;
  }
}

int main() {
  try {
    // Example matrices
    matrix mat1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
    matrix mat2 = {{1, 2, 3, 4, 5}, {6, 7, 8, 9, 10}, {11, 12, 13, 14, 15}};
    int numThreads = 10; // Change this as needed

    // Result matrix
    matrix result;

    // Perform matrix multiplication
    auto start = chrono::high_resolution_clock::now(); // start timer
    multiplyMatrices(mat1, mat2, result, numThreads);
    auto end = chrono::high_resolution_clock::now(); // end timerstart

    // calculate duration
    chrono::duration<double> duration = end - start;

    cout << "Result matrix (example):" << endl;
    displayMatrix(result);

    cout << "Execution time: " << duration.count() * 1000 << " milliseconds."
         << endl
         << endl;

    // Testing
    matrix matrix1 = {
        {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
         16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
        {31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
         46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60},
        {61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
         76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90},
        {91,  92,  93,  94,  95,  96,  97,  98,  99,  100,
         101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
         111, 112, 113, 114, 115, 116, 117, 118, 119, 120},
        {121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
         131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
         141, 142, 143, 144, 145, 146, 147, 148, 149, 150},
        {151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
         161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
         171, 172, 173, 174, 175, 176, 177, 178, 179, 180},
        {181, 182, 183, 184, 185, 186, 187, 188, 189, 190,
         191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
         201, 202, 203, 204, 205, 206, 207, 208, 209, 210},
        {211, 212, 213, 214, 215, 216, 217, 218, 219, 220,
         221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
         231, 232, 233, 234, 235, 236, 237, 238, 239, 240},
        {241, 242, 243, 244, 245, 246, 247, 248, 249, 250,
         251, 252, 253, 254, 255, 256, 257, 258, 259, 260,
         261, 262, 263, 264, 265, 266, 267, 268, 269, 270},
        {271, 272, 273, 274, 275, 276, 277, 278, 279, 280,
         281, 282, 283, 284, 285, 286, 287, 288, 289, 290,
         291, 292, 293, 294, 295, 296, 297, 298, 299, 300},
        {301, 302, 303, 304, 305, 306, 307, 308, 309, 310,
         311, 312, 313, 314, 315, 316, 317, 318, 319, 320,
         321, 322, 323, 324, 325, 326, 327, 328, 329, 330},
        {331, 332, 333, 334, 335, 336, 337, 338, 339, 340,
         341, 342, 343, 344, 345, 346, 347, 348, 349, 350,
         351, 352, 353, 354, 355, 356, 357, 358, 359, 360},
        {361, 362, 363, 364, 365, 366, 367, 368, 369, 370,
         371, 372, 373, 374, 375, 376, 377, 378, 379, 380,
         381, 382, 383, 384, 385, 386, 387, 388, 389, 390},
        {391, 392, 393, 394, 395, 396, 397, 398, 399, 400,
         401, 402, 403, 404, 405, 406, 407, 408, 409, 410,
         411, 412, 413, 414, 415, 416, 417, 418, 419, 420},
        {421, 422, 423, 424, 425, 426, 427, 428, 429, 430,
         431, 432, 433, 434, 435, 436, 437, 438, 439, 440,
         441, 442, 443, 444, 445, 446, 447, 448, 449, 450},
        {451, 452, 453, 454, 455, 456, 457, 458, 459, 460,
         461, 462, 463, 464, 465, 466, 467, 468, 469, 470,
         471, 472, 473, 474, 475, 476, 477, 478, 479, 480},
        {481, 482, 483, 484, 485, 486, 487, 488, 489, 490,
         491, 492, 493, 494, 495, 496, 497, 498, 499, 500,
         501, 502, 503, 504, 505, 506, 507, 508, 509, 510},
        {511, 512, 513, 514, 515, 516, 517, 518, 519, 520,
         521, 522, 523, 524, 525, 526, 527, 528, 529, 530,
         531, 532, 533, 534, 535, 536, 537, 538, 539, 540},
        {541, 542, 543, 544, 545, 546, 547, 548, 549, 550,
         551, 552, 553, 554, 555, 556, 557, 558, 559, 560,
         561, 562, 563, 564, 565, 566, 567, 568, 569, 570},
    };

    matrix matrix2 = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
         31, 32, 33, 34, 35, 36, 37, 38, 39, 40},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
         31, 32, 33, 34, 35, 36, 37, 38, 39, 40},
        {41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
         51, 52, 53, 54, 55, 56, 57, 58, 59, 60},
        {61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
         71, 72, 73, 74, 75, 76, 77, 78, 79, 80},
        {81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
         91, 92, 93, 94, 95, 96, 97, 98, 99, 100},
        {101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
         111, 112, 113, 114, 115, 116, 117, 118, 119, 120},
        {121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
         131, 132, 133, 134, 135, 136, 137, 138, 139, 140},
        {141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
         151, 152, 153, 154, 155, 156, 157, 158, 159, 160},
        {161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
         171, 172, 173, 174, 175, 176, 177, 178, 179, 180},
        {181, 182, 183, 184, 185, 186, 187, 188, 189, 190,
         191, 192, 193, 194, 195, 196, 197, 198, 199, 200},
        {201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
         211, 212, 213, 214, 215, 216, 217, 218, 219, 220},
        {221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
         231, 232, 233, 234, 235, 236, 237, 238, 239, 240},
        {241, 242, 243, 244, 245, 246, 247, 248, 249, 250,
         251, 252, 253, 254, 255, 256, 257, 258, 259, 260},
        {261, 262, 263, 264, 265, 266, 267, 268, 269, 270,
         271, 272, 273, 274, 275, 276, 277, 278, 279, 280},
        {281, 282, 283, 284, 285, 286, 287, 288, 289, 290,
         291, 292, 293, 294, 295, 296, 297, 298, 299, 300},
        {301, 302, 303, 304, 305, 306, 307, 308, 309, 310,
         311, 312, 313, 314, 315, 316, 317, 318, 319, 320},
        {321, 322, 323, 324, 325, 326, 327, 328, 329, 330,
         331, 332, 333, 334, 335, 336, 337, 338, 339, 340},
        {341, 342, 343, 344, 345, 346, 347, 348, 349, 350,
         351, 352, 353, 354, 355, 356, 357, 358, 359, 360},
        {361, 362, 363, 364, 365, 366, 367, 368, 369, 370,
         371, 372, 373, 374, 375, 376, 377, 378, 379, 380},
        {381, 382, 383, 384, 385, 386, 387, 388, 389, 390,
         391, 392, 393, 394, 395, 396, 397, 398, 399, 400},
        {401, 402, 403, 404, 405, 406, 407, 408, 409, 410,
         411, 412, 413, 414, 415, 416, 417, 418, 419, 420},
        {421, 422, 423, 424, 425, 426, 427, 428, 429, 430,
         431, 432, 433, 434, 435, 436, 437, 438, 439, 440},
        {441, 442, 443, 444, 445, 446, 447, 448, 449, 450,
         451, 452, 453, 454, 455, 456, 457, 458, 459, 460},
        {461, 462, 463, 464, 465, 466, 467, 468, 469, 470,
         471, 472, 473, 474, 475, 476, 477, 478, 479, 480},
        {481, 482, 483, 484, 485, 486, 487, 488, 489, 490,
         491, 492, 493, 494, 495, 496, 497, 498, 499, 500},
        {501, 502, 503, 504, 505, 506, 507, 508, 509, 510,
         511, 512, 513, 514, 515, 516, 517, 518, 519, 520},
        {521, 522, 523, 524, 525, 526, 527, 528, 529, 530,
         531, 532, 533, 534, 535, 536, 537, 538, 539, 540},
        {541, 542, 543, 544, 545, 546, 547, 548, 549, 550,
         551, 552, 553, 554, 555, 556, 557, 558, 559, 560},
    };

    matrix _result;

    // 1, 4, 8, 12, 16, 20, 24, 30
    int threadCount = 30; // you can change this value

    auto _start = chrono::high_resolution_clock::now(); // start timer
    multiplyMatrices(matrix1, matrix2, _result, threadCount);
    auto _end = chrono::high_resolution_clock::now(); // end timerstart

    // calculate duration
    chrono::duration<double> _duration = _end - _start;

    /* cout << "Result matrix :" << endl; */
    /* displayMatrix(_result); */

    cout << "Number of threads: " << threadCount << endl;
    cout << "Execution time: " << _duration.count() * 1000 << " milliseconds."
         << endl;
  } catch (runtime_error &err) {
    cout << err.what() << endl;
  }

  return 0;
}
