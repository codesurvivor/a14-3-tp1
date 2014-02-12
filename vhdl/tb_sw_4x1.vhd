-- Testbench created online at:
--   www.doulos.com/knowhow/perl/testbench_creation/
-- Copyright Doulos Ltd
-- SD, 03 November 2002

library IEEE;
use IEEE.Std_logic_1164.all;
use IEEE.Numeric_Std.all;
use work.all;

entity tb_sw_4x1 is
end;

architecture bench of tb_sw_4x1 is

  component sw_4x1
		Generic (
			fifo_size : integer := 8);
      Port ( 
  		  rst : in std_logic;
  		  clk : in std_logic;
		  arbiter_mode : in std_logic_vector (1 downto 0);
  		  in0 : in  STD_LOGIC_VECTOR (39 downto 0);
  		  in0_req : in  STD_LOGIC;
  		  in0_ack : out  STD_LOGIC;
  		  in1 : in  STD_LOGIC_VECTOR (39 downto 0);
  		  in1_req : in  STD_LOGIC;
  		  in1_ack : out  STD_LOGIC;
  		  in2 : in  STD_LOGIC_VECTOR (39 downto 0);
  		  in2_req : in  STD_LOGIC;
  		  in2_ack : out  STD_LOGIC;
  		  in3 : in  STD_LOGIC_VECTOR (39 downto 0);
  		  in3_req : in  STD_LOGIC;
  		  in3_ack : out  STD_LOGIC;
  		  out0 : out  STD_LOGIC_VECTOR (39 downto 0);
  		  out0_req : out  STD_LOGIC;
  		  out0_ack : in  STD_LOGIC
  		);
  end component;
  
  component traffic_generator
	 Generic ( 
				id : std_logic_vector (4 downto 0);
				target : std_logic_vector (1 downto 0);
				cycles_per_msg : integer
				);
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           dout : out std_logic_vector(39 downto 0);
			  dout_req : out std_logic;
			  dout_ack : in std_logic
			);
	end component traffic_generator;
	
	component traffic_acceptor
      Port ( clk : in  STD_LOGIC;
  			  rst : in std_logic;
             din : in std_logic_vector(39 downto 0);
  			  din_req : in std_logic;
  			  din_ack : out std_logic
  			);
  end component;

  signal rst: std_logic;
  signal clk: std_logic;
  signal in0: STD_LOGIC_VECTOR (39 downto 0);
  signal in0_req: STD_LOGIC;
  signal in0_ack: STD_LOGIC;
  signal in1: STD_LOGIC_VECTOR (39 downto 0);
  signal in1_req: STD_LOGIC;
  signal in1_ack: STD_LOGIC;
  signal in2: STD_LOGIC_VECTOR (39 downto 0);
  signal in2_req: STD_LOGIC;
  signal in2_ack: STD_LOGIC;
  signal in3: STD_LOGIC_VECTOR (39 downto 0);
  signal in3_req: STD_LOGIC;
  signal in3_ack: STD_LOGIC;
  signal out0: STD_LOGIC_VECTOR (39 downto 0);
  signal out0_req: STD_LOGIC;
  signal out0_ack: STD_LOGIC ;

  constant clock_period: time := 10 ns;
  signal stop_the_clock: boolean := false;
  
  constant lru : std_logic_vector (1 downto 0) := "01";
  constant fixed : std_logic_vector (1 downto 0) := "00";
  constant rand : std_logic_vector (1 downto 0) := "10";
  constant round : std_logic_vector (1 downto 0) := "11";
  
begin

  uut: sw_4x1 
			generic map (fifo_size => 0)
			port map ( rst      => rst,
                      clk      => clk,
							 arbiter_mode => fixed,
                      in0      => in0,
                      in0_req  => in0_req,
                      in0_ack  => in0_ack,
                      in1      => in1,
                      in1_req  => in1_req,
                      in1_ack  => in1_ack,
                      in2      => in2,
                      in2_req  => in2_req,
                      in2_ack  => in2_ack,
                      in3      => in3,
                      in3_req  => in3_req,
                      in3_ack  => in3_ack,
                      out0     => out0,
                      out0_req => out0_req,
                      out0_ack => out0_ack );
							 
	tg0 : traffic_generator generic map ( id             => "00000",
                                       target         => "01",
                                       cycles_per_msg =>  2)
                            port map ( clk            => clk,
                                       rst            => rst,
                                       dout           => in0,
                                       dout_req       => in0_req,
                                       dout_ack       => in0_ack );
							 
	tg1 : traffic_generator generic map ( id             => "00001",
                                       target         => "01",
                                       cycles_per_msg =>  2)
                            port map ( clk            => clk,
                                       rst            => rst,
                                       dout           => in1,
                                       dout_req       => in1_req,
                                       dout_ack       => in1_ack );
							 
	tg2 : traffic_generator generic map ( id             => "00010",
                                       target         => "01",
                                       cycles_per_msg =>  2)
                            port map ( clk            => clk,
                                       rst            => rst,
                                       dout           => in2,
                                       dout_req       => in2_req,
                                       dout_ack       => in2_ack );
							 
	tg3 : traffic_generator generic map ( id             => "00011",
                                       target         => "01",
                                       cycles_per_msg =>  2)
                            port map ( clk            => clk,
                                       rst            => rst,
                                       dout           => in3,
                                       dout_req       => in3_req,
                                       dout_ack       => in3_ack );
													
	acc: traffic_acceptor port map ( clk     => clk,
                                   rst     => rst,
                                   din     => out0,
                                   din_req => out0_req,
                                   din_ack => out0_ack );

  stimulus: process
  begin
  
    -- Put initialisation code here
		rst <= '1';
      wait for clock_period*10;
		rst <= '0';

    -- Put test bench stimulus code here
	 wait for clock_period*3000;

    wait;
  end process;

	clocking: process
  begin
    while not stop_the_clock loop
      clk <= '0', '1' after clock_period / 2;
      wait for clock_period;
    end loop;
    wait;
  end process;

end;