library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.all;

entity noc is
	Generic (
			fifo_size_8x4 : integer := 0;
			fifo_size_4x1 : integer := 8);
    Port ( 
		  rst : in std_logic;
		  clk : in std_logic;
		  
		  arbiter_mode_8x4 : in std_logic_vector (1 downto 0);
		  arbiter_mode_4x1 : in std_logic_vector (1 downto 0);
		  
		  in00 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in00_req : in  STD_LOGIC;
		  in00_ack : out  STD_LOGIC;
		  
		  in01 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in01_req : in  STD_LOGIC;
		  in01_ack : out  STD_LOGIC;
		  
		  in02 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in02_req : in  STD_LOGIC;
		  in02_ack : out  STD_LOGIC;  
		  
		  in03 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in03_req : in  STD_LOGIC;
		  in03_ack : out  STD_LOGIC;  
		  
		  in04 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in04_req : in  STD_LOGIC;
		  in04_ack : out  STD_LOGIC;  
		  
		  in05 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in05_req : in  STD_LOGIC;
		  in05_ack : out  STD_LOGIC;  
		  
		  in06 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in06_req : in  STD_LOGIC;
		  in06_ack : out  STD_LOGIC;  
		  
		  in07 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in07_req : in  STD_LOGIC;
		  in07_ack : out  STD_LOGIC;
		  
		  in10 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in10_req : in  STD_LOGIC;
		  in10_ack : out  STD_LOGIC;
		  
		  in11 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in11_req : in  STD_LOGIC;
		  in11_ack : out  STD_LOGIC;
		  
		  in12 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in12_req : in  STD_LOGIC;
		  in12_ack : out  STD_LOGIC;  
		  
		  in13 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in13_req : in  STD_LOGIC;
		  in13_ack : out  STD_LOGIC;  
		  
		  in14 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in14_req : in  STD_LOGIC;
		  in14_ack : out  STD_LOGIC;  
		  
		  in15 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in15_req : in  STD_LOGIC;
		  in15_ack : out  STD_LOGIC;  
		  
		  in16 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in16_req : in  STD_LOGIC;
		  in16_ack : out  STD_LOGIC;  
		  
		  in17 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in17_req : in  STD_LOGIC;
		  in17_ack : out  STD_LOGIC;
		  
		  in20 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in20_req : in  STD_LOGIC;
		  in20_ack : out  STD_LOGIC;
		  
		  in21 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in21_req : in  STD_LOGIC;
		  in21_ack : out  STD_LOGIC;
		  
		  in22 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in22_req : in  STD_LOGIC;
		  in22_ack : out  STD_LOGIC;  
		  
		  in23 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in23_req : in  STD_LOGIC;
		  in23_ack : out  STD_LOGIC;  
		  
		  in24 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in24_req : in  STD_LOGIC;
		  in24_ack : out  STD_LOGIC;  
		  
		  in25 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in25_req : in  STD_LOGIC;
		  in25_ack : out  STD_LOGIC;  
		  
		  in26 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in26_req : in  STD_LOGIC;
		  in26_ack : out  STD_LOGIC;  
		  
		  in27 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in27_req : in  STD_LOGIC;
		  in27_ack : out  STD_LOGIC;
		  
		  in30 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in30_req : in  STD_LOGIC;
		  in30_ack : out  STD_LOGIC;
		  
		  in31 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in31_req : in  STD_LOGIC;
		  in31_ack : out  STD_LOGIC;
		  
		  in32 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in32_req : in  STD_LOGIC;
		  in32_ack : out  STD_LOGIC;  
		  
		  in33 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in33_req : in  STD_LOGIC;
		  in33_ack : out  STD_LOGIC;  
		  
		  in34 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in34_req : in  STD_LOGIC;
		  in34_ack : out  STD_LOGIC;  
		  
		  in35 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in35_req : in  STD_LOGIC;
		  in35_ack : out  STD_LOGIC;  
		  
		  in36 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in36_req : in  STD_LOGIC;
		  in36_ack : out  STD_LOGIC;  
		  
		  in37 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in37_req : in  STD_LOGIC;
		  in37_ack : out  STD_LOGIC;
		  
		  out0 : out  STD_LOGIC_VECTOR (39 downto 0);
		  out0_req : out  STD_LOGIC;
		  out0_ack : in  STD_LOGIC;
		  
		  out1 : out  STD_LOGIC_VECTOR (39 downto 0);
		  out1_req : out  STD_LOGIC;
		  out1_ack : in  STD_LOGIC;
		  
		  out2 : out  STD_LOGIC_VECTOR (39 downto 0);
		  out2_req : out  STD_LOGIC;
		  out2_ack : in  STD_LOGIC;
		  
		  out3 : out  STD_LOGIC_VECTOR (39 downto 0);
		  out3_req : out  STD_LOGIC;
		  out3_ack : in  STD_LOGIC
		);
end noc;

architecture Behavioral of noc is
	---types
	type handshake_4x4 is array (3 downto 0) of std_logic_vector(3 downto 0);
	type handshake_4x8 is array (3 downto 0) of std_logic_vector(7 downto 0);
	type data_4 is array (3 downto 0) of std_logic_vector (39 downto 0);
	type data_8 is array (7 downto 0) of std_logic_vector (39 downto 0);
	type data_4x4 is array (3 downto 0) of data_4;
	type data_4x8 is array (3 downto 0) of data_8; 
	--- in signals
	signal din : data_4x8;
	signal in_acks : handshake_4x8;
	signal in_reqs : handshake_4x8;
	--- middle signals
	signal mid_entry : data_4x4;
	signal mid_acks : handshake_4x4;
	signal mid_reqs : handshake_4x4;
	--- out signals
	signal dout : data_4;
	signal out_acks : std_logic_vector(3 downto 0);
	signal out_reqs : std_logic_vector(3 downto 0);
begin
	gen_sw_8x4: 
   for i in 0 to 3 generate
		sw: entity sw_8x4
			generic map (fifo_size => fifo_size_8x4)
			port map (
				clk => clk,
				rst => rst,
				
				arbiter_mode => arbiter_mode_8x4,
				
				in0 =>din(i)(0),
				in0_req =>in_reqs(i)(0),
				in0_ack =>in_acks(i)(0),
				
				in1 =>din(i)(1),
				in1_req =>in_reqs(i)(1),
				in1_ack =>in_acks(i)(1),
				
				in2 =>din(i)(2),
				in2_req =>in_reqs(i)(2),
				in2_ack =>in_acks(i)(2),
				
				in3 =>din(i)(3),
				in3_req =>in_reqs(i)(3),
				in3_ack =>in_acks(i)(3),
				
				in4 =>din(i)(4),
				in4_req =>in_reqs(i)(4),
				in4_ack =>in_acks(i)(4),
				
				in5 =>din(i)(5),
				in5_req =>in_reqs(i)(5),
				in5_ack =>in_acks(i)(5),
				
				in6 =>din(i)(6),
				in6_req =>in_reqs(i)(6),
				in6_ack =>in_acks(i)(6),
				
				in7 =>din(i)(7),
				in7_req =>in_reqs(i)(7),
				in7_ack =>in_acks(i)(7),
				
				out0 =>mid_entry(i)(0),
				out0_req =>mid_reqs(i)(0),
				out0_ack =>mid_acks(i)(0),
				
				out1 =>mid_entry(i)(1),
				out1_req =>mid_reqs(i)(1),
				out1_ack =>mid_acks(i)(1),
				
				out2 =>mid_entry(i)(2),
				out2_req =>mid_reqs(i)(2),
				out2_ack =>mid_acks(i)(2),
				
				out3 =>mid_entry(i)(3),
				out3_req =>mid_reqs(i)(3),
				out3_ack =>mid_acks(i)(3)
			);
	end generate;
	
	gen_sw_4x1: 
   for i in 0 to 3 generate
		sw: entity sw_4x1
			generic map (fifo_size => fifo_size_4x1)
			port map (
				clk => clk,
				rst => rst,
				
				arbiter_mode => arbiter_mode_4x1,
				
				in0 =>mid_entry(0)(i),
				in0_req =>mid_reqs(0)(i),
				in0_ack =>mid_acks(0)(i),
				
				in1 =>mid_entry(1)(i),
				in1_req =>mid_reqs(1)(i),
				in1_ack =>mid_acks(1)(i),
				
				in2 =>mid_entry(2)(i),
				in2_req =>mid_reqs(2)(i),
				in2_ack =>mid_acks(2)(i),
				
				in3 =>mid_entry(3)(i),
				in3_req =>mid_reqs(3)(i),
				in3_ack =>mid_acks(3)(i),
				
				out0 =>dout(i),
				out0_req =>out_reqs(i),
				out0_ack =>out_acks(i)
			);
	end generate;
	
	--- Data interface
	din(0)(0) <= in00;
	din(0)(1) <= in01;
	din(0)(2) <= in02;
	din(0)(3) <= in03;
	din(0)(4) <= in04;
	din(0)(5) <= in05;
	din(0)(6) <= in06;
	din(0)(7) <= in07;
	
	din(1)(0) <= in10;
	din(1)(1) <= in11;
	din(1)(2) <= in12;
	din(1)(3) <= in13;
	din(1)(4) <= in14;
	din(1)(5) <= in15;
	din(1)(6) <= in16;
	din(1)(7) <= in17;
	
	din(2)(0) <= in20;
	din(2)(1) <= in21;
	din(2)(2) <= in22;
	din(2)(3) <= in23;
	din(2)(4) <= in24;
	din(2)(5) <= in25;
	din(2)(6) <= in26;
	din(2)(7) <= in27;
	
	din(3)(0) <= in30;
	din(3)(1) <= in31;
	din(3)(2) <= in32;
	din(3)(3) <= in33;
	din(3)(4) <= in34;
	din(3)(5) <= in35;
	din(3)(6) <= in36;
	din(3)(7) <= in37;
	
	out0 <= dout(0);
	out1 <= dout(1);
	out2 <= dout(2);
	out3 <= dout(3);
	
	--- Handshake interface
	in_reqs(0) <= in07_req & in06_req & in05_req & in04_req & in03_req & in02_req & in01_req & in00_req;
	in_reqs(1) <= in17_req & in16_req & in15_req & in14_req & in13_req & in12_req & in11_req & in10_req;
	in_reqs(2) <= in27_req & in26_req & in25_req & in24_req & in23_req & in22_req & in21_req & in20_req;
	in_reqs(3) <= in37_req & in36_req & in35_req & in34_req & in33_req & in32_req & in31_req & in30_req;
	
	in00_ack <= in_acks(0)(0);
	in01_ack <= in_acks(0)(1);
	in02_ack <= in_acks(0)(2);
	in03_ack <= in_acks(0)(3);
	in04_ack <= in_acks(0)(4);
	in05_ack <= in_acks(0)(5);
	in06_ack <= in_acks(0)(6);
	in07_ack <= in_acks(0)(7);
	
	in10_ack <= in_acks(1)(0);
	in11_ack <= in_acks(1)(1);
	in12_ack <= in_acks(1)(2);
	in13_ack <= in_acks(1)(3);
	in14_ack <= in_acks(1)(4);
	in15_ack <= in_acks(1)(5);
	in16_ack <= in_acks(1)(6);
	in17_ack <= in_acks(1)(7);
	
	in20_ack <= in_acks(2)(0);
	in21_ack <= in_acks(2)(1);
	in22_ack <= in_acks(2)(2);
	in23_ack <= in_acks(2)(3);
	in24_ack <= in_acks(2)(4);
	in25_ack <= in_acks(2)(5);
	in26_ack <= in_acks(2)(6);
	in27_ack <= in_acks(2)(7);
	
	in30_ack <= in_acks(3)(0);
	in31_ack <= in_acks(3)(1);
	in32_ack <= in_acks(3)(2);
	in33_ack <= in_acks(3)(3);
	in34_ack <= in_acks(3)(4);
	in35_ack <= in_acks(3)(5);
	in36_ack <= in_acks(3)(6);
	in37_ack <= in_acks(3)(7);
	
	out_acks <= out3_ack & out2_ack & out1_ack & out0_ack;
	
	out0_req <= out_reqs(0);
	out1_req <= out_reqs(1);
	out2_req <= out_reqs(2);
	out3_req <= out_reqs(3);
	
end Behavioral;

