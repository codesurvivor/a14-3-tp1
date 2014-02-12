library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.all;

entity sw_8x4 is
	Generic (
		fifo_size : integer;
		address_base : integer range 0 to 38 := 36
			);
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
		  
		  in4 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in4_req : in  STD_LOGIC;
		  in4_ack : out  STD_LOGIC;  
		  
		  in5 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in5_req : in  STD_LOGIC;
		  in5_ack : out  STD_LOGIC;  
		  
		  in6 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in6_req : in  STD_LOGIC;
		  in6_ack : out  STD_LOGIC;  
		  
		  in7 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in7_req : in  STD_LOGIC;
		  in7_ack : out  STD_LOGIC;
		  
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
end sw_8x4;

architecture Behavioral of sw_8x4 is

	--- Data
	type data_array_type_x4 is array (3 downto 0) of std_logic_vector (39 downto 0);
	type data_array_type_x8 is array (8 downto 0) of std_logic_vector (39 downto 0);
	type data_array_type_x8x4 is array (7 downto 0) of data_array_type_x4;
	signal dout : data_array_type_x4 ;
	signal din : data_array_type_x8 ;
	signal fifo_entry : data_array_type_x8x4 ;
	--- Handshake
	type handshake_array_type_x4 is array (3 downto 0) of std_logic;
	type handshake_array_type_x8x4 is array (8 downto 0) of handshake_array_type_x4;
	signal in_acks : std_logic_vector (7 downto 0) ;
	signal in_reqs : std_logic_vector (7 downto 0) ;
	signal out_acks : std_logic_vector (3 downto 0) ;
	signal out_reqs : std_logic_vector (3 downto 0) ;
	signal fifo_acks : handshake_array_type_x8x4;
	signal fifo_reqs : handshake_array_type_x8x4;
	
begin

	gen_routers: 
   for i in 0 to 7 generate
		router: entity router_1x4
			generic map (address_base => address_base)
			port map (
				clk => clk,
				rst => rst,
				
				in0 => din(i),
				in0_ack => in_acks(i),
				in0_req => in_reqs(i),
				
				out0 => fifo_entry(i)(0),
				out0_ack => fifo_acks(i)(0),
				out0_req => fifo_reqs(i)(0),
				
				out1	=> fifo_entry(i)(1),
				out1_ack => fifo_acks(i)(1),
				out1_req => fifo_reqs(i)(1),
				
				out2 => fifo_entry(i)(2),
				out2_ack => fifo_acks(i)(2),
				out2_req => fifo_reqs(i)(2),
				
				out3	=> fifo_entry(i)(3),
				out3_ack => fifo_acks(i)(3),
				out3_req => fifo_reqs(i)(3)
			);
	end generate;
	
	gen_sw: 
   for i in 0 to 3 generate
		sw: entity sw_8x1
			generic map (fifo_size => fifo_size)
			port map (
				clk => clk,
				rst => rst,
				
				arbiter_mode => arbiter_mode,
				
				in0 =>fifo_entry(0)(i),
				in0_req =>fifo_reqs(0)(i),
				in0_ack =>fifo_acks(0)(i),
				
				in1 =>fifo_entry(1)(i),
				in1_req =>fifo_reqs(1)(i),
				in1_ack =>fifo_acks(1)(i),
				
				in2 =>fifo_entry(2)(i),
				in2_req =>fifo_reqs(2)(i),
				in2_ack =>fifo_acks(2)(i),
				
				in3 =>fifo_entry(3)(i),
				in3_req =>fifo_reqs(3)(i),
				in3_ack =>fifo_acks(3)(i),
				
				in4 =>fifo_entry(4)(i),
				in4_req =>fifo_reqs(4)(i),
				in4_ack =>fifo_acks(4)(i),
				
				in5 =>fifo_entry(5)(i),
				in5_req =>fifo_reqs(5)(i),
				in5_ack =>fifo_acks(5)(i),
				
				in6 =>fifo_entry(6)(i),
				in6_req =>fifo_reqs(6)(i),
				in6_ack =>fifo_acks(6)(i),
				
				in7 =>fifo_entry(7)(i),
				in7_req =>fifo_reqs(7)(i),
				in7_ack =>fifo_acks(7)(i),
				
				out0 =>dout(i),
				out0_req =>out_reqs(i),
				out0_ack =>out_acks(i)
			);
	end generate;
	
	--- Data interface
	din(0) <= in0;
	din(1) <= in1;
	din(2) <= in2;
	din(3) <= in3;
	din(4) <= in4;
	din(5) <= in5;
	din(6) <= in6;
	din(7) <= in7;
	
	out0 <= dout(0);
	out1 <= dout(1);
	out2 <= dout(2);
	out3 <= dout(3);
	
	--- Handshake interface
	in_reqs <= in7_req & in6_req & in5_req & in4_req & in3_req & in2_req & in1_req & in0_req;
	
	in0_ack <= in_acks(0);
	in1_ack <= in_acks(1);
	in2_ack <= in_acks(2);
	in3_ack <= in_acks(3);
	in4_ack <= in_acks(4);
	in5_ack <= in_acks(5);
	in6_ack <= in_acks(6);
	in7_ack <= in_acks(7);
	
	out_acks <= out3_ack & out2_ack & out1_ack & out0_ack;
	
	out0_req <= out_reqs(0);
	out1_req <= out_reqs(1);
	out2_req <= out_reqs(2);
	out3_req <= out_reqs(3);

end Behavioral;

