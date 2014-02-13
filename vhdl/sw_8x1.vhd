library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.all;

entity sw_8x1 is
	Generic (fifo_size : integer);
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
		  out0_ack : in  STD_LOGIC
		);
end sw_8x1;

architecture Behavioral of sw_8x1 is

	--- FIFO interfaces
	--- Data
	type data_array_type is array (7 downto 0) of std_logic_vector (39 downto 0);
	signal dout : data_array_type ;
	signal din : data_array_type ;
	--- Handshake
	signal rd_ack : std_logic_vector (7 downto 0) ;
	signal rd_req : std_logic_vector (7 downto 0) ;
	signal wr_req : std_logic_vector (7 downto 0) ;
	signal wr_ack : std_logic_vector (7 downto 0) ;
	
	--- Arbiter interfaces
	signal choice_fixed : std_logic_vector (7 downto 0);
	signal choice_lru : std_logic_vector (7 downto 0);
	signal choice_rand : std_logic_vector (7 downto 0);
	signal choice_round : std_logic_vector (7 downto 0);
	signal choice : std_logic_vector (7 downto 0);
	signal empty : std_logic_vector (7 downto 0);
	signal choice_used : std_logic;
	
	--- Output handshake register
	signal out0_req_tmp : std_logic := '0';
	
begin
	  
	lru_inst: entity arbiter_lru_8 port map (
          clk => clk,
			 rst => rst,
          empty => empty,
			 prechoice => choice_lru,
			 choice => choice,
			 choice_used => choice_used
	  );
	  
	  fixed_inst: entity arbiter_fixed_8 port map (
          clk => clk,
			 rst => rst,
          empty => empty,
			 prechoice => choice_fixed,
			 choice => choice,
			 choice_used => choice_used
	  );
	  
	  rand_inst: entity arbiter_rand_8 port map (
          clk => clk,
			 rst => rst,
          empty => empty,
			 prechoice => choice_rand,
			 choice => choice,
			 choice_used => choice_used
	  );
	  
	  round_inst: entity arbiter_round_8 port map (
          clk => clk,
			 rst => rst,
          empty => empty,
			 prechoice => choice_round,
			 choice => choice,
			 choice_used => choice_used
	  );

	gen_fifos: 
   for i in 0 to 7 generate
		fifo_i: entity fifo
			generic map (nb_stages => fifo_size)
			port map (
				clk => clk,
				rst => rst,
				
				din => din(i),
				wr_req => wr_req(i),
				wr_ack => wr_ack(i),
				
				dout => dout(i),
				rd_req => rd_req(i),
				rd_ack => rd_ack(i)
			);
	end generate;
	
	din(0) <= in0;
	din(1) <= in1;
	din(2) <= in2;
	din(3) <= in3;
	din(4) <= in4;
	din(5) <= in5;
	din(6) <= in6;
	din(7) <= in7;
	
	wr_req <= in7_req & in6_req & in5_req & in4_req & in3_req & in2_req & in1_req & in0_req;
	
	in0_ack <= wr_ack(0);
	in1_ack <= wr_ack(1);
	in2_ack <= wr_ack(2);
	in3_ack <= wr_ack(3);
	in4_ack <= wr_ack(4);
	in5_ack <= wr_ack(5);
	in6_ack <= wr_ack(6);
	in7_ack <= wr_ack(7);
	
	empty <= 	(not (rd_req(7) xor rd_ack(7))) 
				& 	(not (rd_req(6) xor rd_ack(6))) 
				& 	(not (rd_req(5) xor rd_ack(5))) 
				& 	(not (rd_req(4) xor rd_ack(4)))
				&	(not (rd_req(3) xor rd_ack(3))) 
				& 	(not (rd_req(2) xor rd_ack(2))) 
				& 	(not (rd_req(1) xor rd_ack(1))) 
				& 	(not (rd_req(0) xor rd_ack(0)));
				
	
	--- Choice mux
	with arbiter_mode select
	choice <= 	choice_fixed when "00",
					choice_lru when "01",
					choice_rand when "10",
					choice_round when others;
	
	choice_used <= '1' when (choice /= x"00") and (out0_ack = out0_req_tmp) else '0';
	
	--- Data movement process
	process (clk)
	begin
		if rising_edge(clk) then
			if rst='1' then
				rd_ack <= x"00";
				out0_req_tmp <= '0';
			elsif choice(0)='1' and (out0_ack = out0_req_tmp) then
				out0 <= dout(0);
				rd_ack(0) <= not rd_ack(0);
				out0_req_tmp <= not out0_req_tmp;
			elsif choice(1)='1' and (out0_ack = out0_req_tmp) then
				out0 <= dout(1);
				rd_ack(1) <= not rd_ack(1);
				out0_req_tmp <= not out0_req_tmp;
			elsif choice(2)='1' and (out0_ack = out0_req_tmp) then
				out0 <= dout(2);
				rd_ack(2) <= not rd_ack(2);
				out0_req_tmp <= not out0_req_tmp;
			elsif choice(3)='1' and (out0_ack = out0_req_tmp) then
				out0 <= dout(3);
				rd_ack(3) <= not rd_ack(3);
				out0_req_tmp <= not out0_req_tmp;
			elsif choice(4)='1' and (out0_ack = out0_req_tmp) then
				out0 <= dout(4);
				rd_ack(4) <= not rd_ack(4);
				out0_req_tmp <= not out0_req_tmp;
			elsif choice(5)='1' and (out0_ack = out0_req_tmp) then
				out0 <= dout(5);
				rd_ack(5) <= not rd_ack(5);
				out0_req_tmp <= not out0_req_tmp;
			elsif choice(6)='1' and (out0_ack = out0_req_tmp) then
				out0 <= dout(6);
				rd_ack(6) <= not rd_ack(6);
				out0_req_tmp <= not out0_req_tmp;
			elsif choice(7)='1' and (out0_ack = out0_req_tmp) then
				out0 <= dout(7);
				rd_ack(7) <= not rd_ack(7);
				out0_req_tmp <= not out0_req_tmp;
			end if;
		end if;
	end process;
	
	out0_req <= out0_req_tmp;

end Behavioral;

