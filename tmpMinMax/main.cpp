// #include <iostream>
// #include <vector>
// #include <algorithm>
// #include <numeric>
// #include <future>
// // #include <boost/thread.hpp>
// // #include <boost/chrono.hpp>

// int heurastic() {
// 	static int i = 0;

// 	i++;
// 	std::cout << i;
// 	return i;
// }
// // template <int, int>
// int MinMax(int deapth, unsigned int &place) {
// 	int curentRating = 0;
// 	int v = -1000;
//     std::vector<std::future<int>> futures;
// 	unsigned int OPplace;

// 	if (deapth == 0)
// 		return heurastic();
    
// 	for (size_t i = 0; i < 4; i++) {
//         std::cout << '-';
//         if (i == 0) {
//             curentRating = -MinMax(deapth - 1, OPplace);        
//         } else {
 
// //   for(int i = 0; i < 10; ++i) {
// //      futures.push_back (std::async(twice, i));
// //    }

// //    //retrive and print the value stored in the future
// //    for(auto &e : futures) {
// //      std::cout << e.get() << std::endl;
// //    }
//         }
        
//         if (curentRating > v) {
//             v = curentRating;
//             place = i;
//         }
//         // auto handle = std::async(std::launch::async, MinMax, deapth - 1, std::ref(OPplace));
//         // curentRating = -handle.get();
//         // std::cout << "handle :" << -handle.get() << std::endl;

//         // std::cout << "handle :" << curentRating << std::endl;
//     }
// 	return v;
// }

// int	main() {
// 	unsigned int place;
//     auto start = std::chrono::high_resolution_clock::now();
//     int res = MinMax(2, place);
//     auto end = std::chrono::high_resolution_clock::now();
//     // boost::thread threads[10];
// 	std::cout << "\nMinMax : " << res << " place : " << place << '\n';
// 	std::cout << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
// 	return 0;
// }
 
// // int parallel_sum(RandomIt beg, RandomIt end)
// // {
// //     auto len = end - beg;
// //     if (len < 1000)
// //         return std::accumulate(beg, end, 0);
 
//     RandomIt mid = beg + len/2;
//     auto handle = std::async(std::launch::async,
//                              parallel_sum<RandomIt>, mid, end);
//     int sum = parallel_sum(beg, mid);
//     return sum + handle.get();
// }
 
// int main()
// {
//     std::vector<int> v(10000, 1);
//     std::cout << "The sum is " << parallel_sum(v.begin(), v.end()) << '\n';
// }
