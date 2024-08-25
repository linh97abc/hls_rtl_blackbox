`timescale 1ns/1ps

module hls_black_box (
    input ap_clk,
    input ap_rst,
    input ap_ce,//
    output ap_idle,
    input ap_start,
    output reg ap_ready,
    output reg ap_done,
    input ap_continue,//
    
    input  cmd,
    input [4:0] x,
    input  y,
    //output [4:0] res_data,
    //output wire res_valid,
    //output wire has_new_val
    output reg [5:0] res
);
    assign ap_idle = ap_ready;

    reg new_step;
    always @(posedge ap_clk)
    if(ap_rst) begin
        res <= 0;
        new_step <= 0;
        ap_done <= 0;
        ap_ready <= 1;
        
    end else begin
        //ap_done <= 1;
        //res[5] <= 1;

        ap_done <= 0;
        ap_ready <= 1;

        if (ap_start & ap_ready) begin
            ap_ready <= 0;
            ap_done <= 1;
            res[5] <= 0;

            if (cmd) begin
                res[4:0] <= x + y;
                new_step <= 1;
            end else begin
                new_step <= 0;
                res[5] <= 1;
            end
        end
    end
endmodule