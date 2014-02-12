library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity router_1x4 is
	 Generic (address_base : integer range 0 to 38 := 36);
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           
			  in0 : in  STD_LOGIC_VECTOR (39 downto 0);
			  in0_req : in  STD_LOGIC;
			  in0_ack : out  STD_LOGIC;
			  
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
end router_1x4;

architecture Behavioral of router_1x4 is
	signal in0_ack_tmp : std_logic := '0';
	signal out0_req_tmp : std_logic := '0';
	signal out1_req_tmp : std_logic := '0';
	signal out2_req_tmp : std_logic := '0';
	signal out3_req_tmp : std_logic := '0';
	
	signal address : std_logic_vector(1 downto 0);
begin

	
	address <= in0(address_base+1 downto address_base);
	
	out0 <= in0;
	out1 <= in0;
	out2 <= in0;
	out3 <= in0;
	
	in0_ack_tmp <= (out0_ack xor out1_ack) xor (out2_ack xor out3_ack); 
	
	process (clk) begin
		if rising_edge(clk) then
			if rst='1' then
				out0_req_tmp <= '0';
				out1_req_tmp <= '0';
				out2_req_tmp <= '0';
				out3_req_tmp <= '0';
			elsif in0_ack_tmp /= in0_req then
				if address="00" and (out0_ack=out0_req_tmp) then
					out0_req_tmp <= not out0_req_tmp;
				elsif address="01" and (out1_ack=out1_req_tmp) then
					out1_req_tmp <= not out1_req_tmp;
				elsif address="10" and (out2_ack=out2_req_tmp) then
					out2_req_tmp <= not out2_req_tmp;
				elsif address="11" and (out3_ack=out3_req_tmp) then
					out3_req_tmp <= not out3_req_tmp;
				end if;
			end if;
		end if;
	end process;
	
	--- Directs outputs to hard outputs
	in0_ack <= in0_ack_tmp;
	out0_req <= out0_req_tmp;
	out1_req <= out1_req_tmp;
	out2_req <= out2_req_tmp;
	out3_req <= out3_req_tmp;

end Behavioral;

