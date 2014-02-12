-- Testbench created online at:
--   www.doulos.com/knowhow/perl/testbench_creation/
-- Copyright Doulos Ltd
-- SD, 03 November 2002

library IEEE;
use IEEE.Std_logic_1164.all;
use IEEE.Numeric_Std.all;
use work.all;

entity tb_fifo is
end;

architecture bench of tb_fifo is

  component fifo
  	generic (nb_stages : integer);
      Port ( 
  			clk : in std_logic;
  			rst : in std_logic;
  			din : in std_logic_vector(39 downto 0);
  			wr_req : in std_logic;
  			wr_ack : out std_logic;
  			dout : out std_logic_vector(39 downto 0);
  			rd_ack : in std_logic;
  			rd_req : out std_logic
  		);
  end component;

  signal clk: std_logic;
  signal rst: std_logic;
  signal din: std_logic_vector(39 downto 0);
  signal wr_req: std_logic;
  signal wr_ack: std_logic;
  signal dout: std_logic_vector(39 downto 0);
  signal rd_ack: std_logic;
  signal rd_req: std_logic ;
  
  -- Clock period definitions
   constant clk_period : time := 10 ns;

begin

  -- Insert values for generic parameters !!
  uut: fifo generic map ( nb_stages =>  8 )
               port map ( clk       => clk,
                          rst       => rst,
                          din       => din,
                          wr_req    => wr_req,
                          wr_ack    => wr_ack,
                          dout      => dout,
                          rd_ack    => rd_ack,
                          rd_req    => rd_req );

	-- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
								  
  stimulus: process
  begin
	 
	-- hold reset state for 100 ns.
		wr_req <= '0';
		rd_ack <= '0';
		rst <= '1';
      wait for clk_period*10;

      -- insert stimulus here 
		rst <= '0';
      wait for clk_period*10;
		din <= X"0000000000";
		wr_req <= '1';
		wait for clk_period*5;
		din <= X"0000000001";
		wr_req <= '0';
		wait for clk_period*5;
		din <= X"0000000002";
		wr_req <= '1';
		wait for clk_period*10;
		din <= X"0000000003";
		wr_req <= '0';
    wait;
  end process;


end;