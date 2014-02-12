library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.all;

entity sw_3x1 is
	Generic (fifo_size : integer);
    Port ( 
		  rst : in std_logic;
		  clk : in std_logic;
		  
		  in0 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in0_req : in  STD_LOGIC;
		  in0_ack : out  STD_LOGIC;
		  
		  in1 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in1_req : in  STD_LOGIC;
		  in1_ack : out  STD_LOGIC;
		  
		  in2 : in  STD_LOGIC_VECTOR (39 downto 0);
		  in2_req : in  STD_LOGIC;
		  in2_ack : out  STD_LOGIC;
		  
		  out0 : out  STD_LOGIC_VECTOR (39 downto 0);
		  out0_req : out  STD_LOGIC;
		  out0_ack : in  STD_LOGIC
		);
end sw_3x1;

architecture Behavioral of sw_3x1 is

	signal rd_ack : std_logic_vector (2 downto 0) ;
	signal rd_req : std_logic_vector (2 downto 0) ;
	signal wr_req : std_logic_vector (2 downto 0) ;
	signal wr_ack : std_logic_vector (2 downto 0) ;
	signal choice : std_logic_vector (2 downto 0) ;
	
	signal empty : std_logic_vector (2 downto 0);
	
	type data_array_type is array (2 downto 0) of std_logic_vector (39 downto 0);
	signal dout : data_array_type ;
	signal din : data_array_type ;
	
	signal out0_req_tmp : std_logic;
	
begin

	arbiter: entity arbiter_fixed port map (
          clk => clk,
			 rst => rst,
          empty => empty,
			 prechoice => choice,
			 choice => "0000",
			 choice_used => '0'
	  );
	
	gen_fifos: 
   for i in 0 to 2 generate
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
	
	wr_req <= in2_req & in1_req & in0_req;
	
	in0_ack <= wr_ack(0);
	in1_ack <= wr_ack(1);
	in2_ack <= wr_ack(2);
	
	empty <= (not (rd_req(2) xor rd_ack(2))) & (not (rd_req(1) xor rd_ack(1))) & (not (rd_req(0) xor rd_ack(0)));
	
	process (clk)
	begin
		if rising_edge(clk) then
			if rst='1' then
				rd_ack <= "000";
				out0_req_tmp <= '0';
			elsif choice(0)='1' and (out0_ack /= out0_req_tmp) then
				out0 <= dout(0);
				rd_ack(0) <= not rd_ack(0);
				out0_req_tmp <= not out0_req_tmp;
			elsif choice(1)='1' and (out0_ack /= out0_req_tmp) then
				out0 <= dout(1);
				rd_ack(1) <= not rd_ack(1);
				out0_req_tmp <= not out0_req_tmp;
			elsif choice(2)='1' and (out0_ack /= out0_req_tmp) then
				out0 <= dout(2);
				rd_ack(2) <= not rd_ack(2);
				out0_req_tmp <= not out0_req_tmp;
			end if;
		end if;
	end process;
	
	out0_req <= out0_req_tmp;

end Behavioral;

