#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <numeric>
#include <algorithm>
int MinMax() {

    return 1;
}

int	main() {
    unsigned int place;
    // int a = 0;
    // int b = INT_MAX;
    auto start = std::chrono::high_resolution_clock::now();
    // minimax(0, 0, true, values, MIN, MAX);;
    std::thread(MinMax);
    std::thread();
    std::thread();
    std::thread();
    std::thread();
    std::thread();
    std::thread();
    std::thread();
    std::thread();
    std::thread();
    std::thread();
    std::thread();
    std::thread();
    std::thread();
    std::thread();
    std::thread();
    std::thread();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
    return 0;
}
/*
              3
    3         2       2
--3-12-8- -2-4-6- -14-5-2

            4
    3       2   4 
--3-12-8- -2- -4-6-14
*/


//
//      
// --12-8-2- -4-6-14--5-2
// Driver Code
// int main()
// {
// 	int values[8] = { 3, 5, 6, 9, 1, 2, 0, -1 };
// 	cout &lt;&lt; &quot;The optimal value is : &quot;
// 		&lt;&lt; minimax(0, 0, true, values, MIN, MAX);;
// 	return 0;
// }

// int o[] = {0,3,6,4,2,1,5,1,1,1};
// int heurastic() {
//     static int i = 0;
//     i++;
//     // if()
//     std::cout << o[i];
//     return o[i];
// }
// #include <numeric>
// #include <algorithm>

// const int MAX = 1000;
// const int MIN = -1000;

// int MinMax(int deapth, bool maximazingPlayer, unsigned int &place, int a, int b) {
//     int score;
//     int v = 1000;
//     unsigned int OPplace;

//     if (deapth == 0)
//         return heurastic();
//     if (maximazingPlayer) {
//         int best = MIN;
//         for (size_t i = 0; i < 3; i++) {
//             std::cout << '-';
//             score = -MinMax(deapth - 1, false, OPplace, a, b);
//             best = std::min(best, score);
// 			b = std::min(b, best);
//             // std::cout << std::endl << "true a: " << a << " b: " << best << " score: " << score << std::endl;
//             if (score < v) {
//                 v = score;
//                 place = o[i];
//             }
//             if(b <= a)
//                 break;
//         }
//         return best;
//     } else {
//         int best = MAX;
//         for (size_t i = 0; i < 3; i++) {
//             std::cout << '-';
//             score = -MinMax(deapth - 1, true, OPplace, a, b);
//             best = std::max(best, score);
// 			a = std::max(a, best);
//             // std::cout << std::endl << "false a: " << a << " b: " << best << " score: " << score << std::endl;
//             if (score < v) {
//                 v = score;
//                 place = o[i];
//             }
//             if (b <= a)
//                 break;
//         }
//         return best;
//     }
// }

// int	main() {
//     unsigned int place;
//     // int a = 0;
//     // int b = INT_MAX;
//     auto start = std::chrono::high_resolution_clock::now();
//     // minimax(0, 0, true, values, MIN, MAX);;
//     int res = MinMax(2, false, place, MAX, MIN);
//     auto end = std::chrono::high_resolution_clock::now();
//     std::cout << "\nMinMax : " << res << " place : " << place << '\n';
//     std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
//     return 0;
// }

//             6
//     6       9       8  
// --3-6-4- -2-9-5- -7-4-8

//              4
//     3        1       4
// --3-6-4- -2-1-5- -7-4-8

// MAX                     13
// MIN    1        5               9          13       
// // MIN -1-2-3-4- -5-6-7-8-     -9-10-11-12- -13-14-15-16 
//                                                                                                         52
//                         4                                                       20                                                        36                                                      52
//         4        8           12          16               20            24             28          32             36              40             44         48               52            56             60          64
// -- -1-2-3-4- -5-6-7-8- -9-10-11-12- -13-14-15-16- - -17-18-19-20- -21-22-23-24- -25-26-27-28- -29-30-31-32- - -33-34-35-36- -37-38-39-40- -41-42-43-44- -45-46-47-48- - -49-50-51-52- -53-54-55-56- -57-58-59-60- -61-62-63-64


//                 11
//         3                  11
//     3      7        11          15
//  1   3   5   7   9     11   13    15
// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
