module testbench;

   initial
   begin
      $dumpfile("RF.vcd");
      $dumpvars(0,testbench);
   end

  reg [4:0] read_reg_1, read_reg_2; 
  reg [4:0] write_reg; 
  reg [31:0] data;
  reg write_enable, clock, reset;

  wire [31:0] out_1, out_2;

  RF rf (
    .read_reg_1(read_reg_1), 
    .read_reg_2(read_reg_2), 
    .write_reg(write_reg), 
    .data(data), 
    .out_1(out_1), 
    .out_2(out_2),
    .write_enable(write_enable), 
    .clock(clock), 
    .reset(reset)
  );

  initial begin
    reset = 1;
    clock = 1;
  end

  always #5 clock = ~clock;

  initial begin 
    // reading a register while it is being updated 
    // should result in the new value appearing after 
    // after the clk posedge
    #10 reset = 0; 
    #10 {read_reg_1, read_reg_2} = {5'b00000, 5'b00001};
    #1; 
    assert(out_1 == 0);
    assert(out_2 == 0);
    #9 {write_reg, data} = {5'b00001, 32'h000000FE};
    #5 write_enable = 1'b1;
    #1; 
    assert(out_1 == 0);
    assert(out_2 == 0);
    #5; 
    assert(out_1 == 0);
    assert(out_2 == 32'h000000FE);

    {write_reg, data} = {5'b11111, 32'h0000FACE};

    // read simultaneously from two registers
    #15 {read_reg_1, read_reg_2} = {5'b11111, 5'b00001};
    #1; 
    assert(out_1 == 32'h0000FACE);
    assert(out_2 == 32'h000000FE);
    #5 reset = 1; 
    #15;
    for (int i = 0; i < 32; i++) begin 
      read_reg_1 = i;
      assert(out_1 == 0);
    end

    // writes should not take effect when write_enable 
    // is not asserted
    #5 {write_enable, write_reg, data} = {1'b0, 5'b00001, 32'h00001337};
    #15 assert(out_2 == 0); 

    #20 $finish;
  end

endmodule
