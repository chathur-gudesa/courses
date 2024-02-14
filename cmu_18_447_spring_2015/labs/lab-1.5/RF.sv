// An implementation of a register file 

module RF(
  input [4:0] read_reg_1, 
  input [4:0] read_reg_2, 
  input [4:0] write_reg,
  input [31:0] data,
  output reg [31:0] out_1, 
  output reg [31:0] out_2,
  input write_enable,
  input clock, 
  input reset
);

  reg [31:0] registers [31:0];

  initial begin 
    registers[0] = 0; 
  end

  always_ff @ (posedge clock) begin
    if (reset) begin 
      for (int i = 0; i < 32; i++) begin 
        registers[i] = 0;
      end
    end

    else if (write_enable && write_reg) begin       
      registers[write_reg] = data;
    end
  end

  always_comb begin 
    out_1 = registers[read_reg_1];
    out_2 = registers[read_reg_2];
  end

endmodule