library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;

entity traffic_generator is
	 Generic ( 
				id : std_logic_vector (4 downto 0);
				target : std_logic_vector (1 downto 0);
				cycles_per_msg : integer := 2);
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           dout : out std_logic_vector(39 downto 0);
			  dout_req : out std_logic;
			  dout_ack : in std_logic
			);
end traffic_generator;

architecture Behavioral of traffic_generator is
	signal counter : integer;
	signal packet_count: unsigned (31 downto 0);
	signal dout_req_tmp : std_logic;
begin
	
	process (clk) begin
		if rising_edge(clk) then
			if rst = '1' then
				dout_req_tmp <= '0';
				counter <= 1;
				packet_count <= x"00000000";
			elsif counter < cycles_per_msg then
				counter <= counter + 1;
			elsif dout_req_tmp = dout_ack then
				counter <= 1;
				packet_count <= packet_count + 1;
				dout_req_tmp <= not dout_req_tmp;
			end if;
		end if;
	end process;
	
	dout_req <= dout_req_tmp;
	
	dout(39) <= '0'; --- Last packet (unused)
	dout(38 downto 37) <= target; ---target id
	dout(36 downto 32) <= id; --- own id (for return)
	dout(31 downto 0) <= std_logic_vector(packet_count); ---payload
				
				
end Behavioral;
