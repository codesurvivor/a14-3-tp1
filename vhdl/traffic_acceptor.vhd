library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;

entity traffic_acceptor is
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           din : in std_logic_vector(39 downto 0);
			  din_req : in std_logic;
			  din_ack : out std_logic
			);
end traffic_acceptor;

architecture Behavioral of traffic_acceptor is
	signal din_ack_tmp : std_logic;
begin
	
	process (clk) begin
		if rising_edge(clk) then
			if rst = '1' then
				din_ack_tmp <= '0';
			elsif din_req /= din_ack_tmp then
				din_ack_tmp <= not din_ack_tmp;
			end if;
		end if;
	end process;
	
	din_ack <= din_ack_tmp;
				
end Behavioral;
