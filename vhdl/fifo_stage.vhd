library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.all;

entity fifo_stage is
    Port ( 
			clk : in std_logic;
			rst : in std_logic;
			
			din : in std_logic_vector(39 downto 0);
			dout : out std_logic_vector(39 downto 0);
	
			pred_req : in std_logic;
			pred_ack : out std_logic;
			succ_req : out std_logic;
			succ_ack : in std_logic
		);
end fifo_stage;

architecture Behavioral of fifo_stage is
	signal pred_ack_tmp : std_logic := '0';
	signal succ_req_tmp : std_logic := '0';
begin
	process (clk)
	begin
		if rising_edge(clk) then
			if rst='1' then
				succ_req_tmp <= '0';
				pred_ack_tmp <= '0';
			elsif (pred_req /= pred_ack_tmp) and (succ_req_tmp = succ_ack) then
				dout <= din;
				succ_req_tmp <= not succ_req_tmp;
				pred_ack_tmp <= not pred_ack_tmp;
			end if;
		end if;
	end process;
	
	pred_ack <= pred_ack_tmp;
	succ_req <= succ_req_tmp;
	
end Behavioral;

