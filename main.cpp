#include <hls_stream.h>
#include <ap_int.h>
#include <iostream>

struct data_in {
  ap_uint<5> x;
  ap_uint<1> y;
};


void hls_dut(hls::stream<data_in> &din, hls::stream<ap_uint<5>> &dout);

int main()
{
    hls::stream<data_in> din;
    hls::stream<ap_uint<5>> dout;

    std::cout << "Start test" << std::endl;

    din.write({1, 1});

    din.write({2, 0});
    din.write({2, 1});
    din.write({4, 1});
    din.write({5, 0});
    din.write({5, 1});

    hls_dut(din, dout);

    while (!dout.empty()) {
        auto d = dout.read();
        std::cout << "dout: " << d << std::endl;    
    }

    std::cout << "End test" << std::endl;

    return 0;
}