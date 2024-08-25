#include "hls_stream.h"
#include <ap_int.h>

struct data_in {
  ap_uint<5> x;
  ap_uint<1> y;
};

struct data_bb_out {
  ap_uint<5> res;
  ap_uint<1> has_new_data;

  //   const data_bb_out& operator=(ap_uint<6> x)
  data_bb_out() = default;
  data_bb_out(ap_uint<6> x) {
    this->res = x.range(4, 0);
    this->has_new_data = x[5];
  }
};

ap_uint<6> hls_black_box(ap_uint<1> cmd, ap_uint<5> x, ap_uint<1> y
                         //,ap_uint<5> &res
) {
  if (cmd) {
    return x + y;
  }

  return 1u << 5;
}

ap_uint<6> hls_black_box_wrapper(ap_uint<1> cmd, ap_uint<5> x, ap_uint<1> y
                                 //,ap_uint<5> &res
) {
#pragma HLS INLINE off
  return hls_black_box(cmd, x, y
                       //, res
  );
}

void hls_dut(hls::stream<data_in> &din, hls::stream<ap_uint<5>> &dout) {
#pragma HLS INTERFACE port = return mode = s_axilite
#pragma HLS INTERFACE port = din mode = axis
#pragma HLS INTERFACE port = dout mode = axis
#pragma HLS AGGREGATE variable = din compact = bit

  //   init_loop: while (!din.empty()) {
  //     din.read();
  //   }

  data_in d;
  unsigned count = 0;
  bool hasNewData = true;
  // #pragma HLS PIPELINE off
main_loop:
  while (count < 5) {

    ap_uint<1> cmd = 0;
    if (hasNewData && !din.empty()) {
      din.read(d);
      cmd = 1;
    }

    ap_uint<5> res;
    // bool hasNewVal = hls_black_box(cmd, d.x, d.y, res);

    data_bb_out bbres = hls_black_box_wrapper(cmd, d.x, d.y
                                              //, res
    );

    hasNewData = bbres.has_new_data;
    if (bbres.has_new_data) {
      // if (hasNewVal) {
      dout.write(bbres.res);
      count++;
    }
  }
}