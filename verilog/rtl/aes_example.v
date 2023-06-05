// SPDX-FileCopyrightText: 2020 Efabless Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// SPDX-License-Identifier: Apache-2.0

`default_nettype none

// TODO: Edit description

/* 
 *-------------------------------------------------------------
 *
 * user_proj_aes
 *
 * This is an example of a (trivially simple) user project,
 * showing how the user project can connect to the logic
 * analyzer, the wishbone bus, and the I/O pads.
 *
 * This project generates an integer count, which is output
 * on the user area GPIO pads (digital output only).  The
 * wishbone connection allows the project to be controlled
 * (start and stop) from the management SoC program.
 *
 * See the testbenches in directory "mprj_counter" for the
 * example programs that drive this user project.  The three
 * testbenches are "io_ports", "la_test1", and "la_test2".
 *
 *-------------------------------------------------------------
 */

module aes_example (
`ifdef USE_POWER_PINS
    inout vccd1,	// User area 1 1.8V supply
    inout vssd1,	// User area 1 digital ground
`endif

    // Wishbone Slave ports (WB MI A)
    input wb_clk_i,
    input wb_rst_i,
    input wbs_stb_i,
    input wbs_cyc_i,
    input wbs_we_i,
    input [3:0] wbs_sel_i,
    input [31:0] wbs_dat_i,
    input [31:0] wbs_adr_i,
    output wbs_ack_o,
    output [31:0] wbs_dat_o

    // Logic Analyzer Signals
    // input  [127:0] la_data_in,
    // output [127:0] la_data_out,
    // input  [127:0] la_oenb,

    // IOs
    // input  [15:0] io_in,
    // output [15:0] io_out,
    // output [15:0] io_oeb,

    // IRQ
    // output [2:0] irq
);

    wire valid;
    wire write_enable;
    wire read_enable;

    reg wbs_ack_o_reg;

    assign valid = wbs_cyc_i && wbs_stb_i;
    assign write_enable = wbs_we_i && valid;
    assign read_enable = ~wbs_we_i && valid;

    aes aes(
        // Clock and reset.
        .clk(wb_clk_i),
        .reset_n(!wb_rst_i),

        // Control.
        .cs(wbs_cyc_i && wbs_stb_i),
        .we(write_enable),

        // Data ports.
        .address(wbs_adr_i[9:2]),
        .write_data(wbs_dat_i),
        .read_data(wbs_dat_o)
    );

    // Ack logic
    always @(posedge wb_clk_i or posedge wb_rst_i) begin
        if (wb_rst_i)
            wbs_ack_o_reg <= 1'b0;
        else if (wbs_cyc_i && wbs_stb_i && ~wbs_ack_o_reg)
            wbs_ack_o_reg <= 1'b1;
        else
            wbs_ack_o_reg <= 1'b0;
    end

    assign wbs_ack_o = wbs_ack_o_reg;

endmodule

`default_nettype none
