library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity arbiter_lru_8 is
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           empty : in  STD_LOGIC_VECTOR (7 downto 0);
           prechoice : out  STD_LOGIC_VECTOR (7 downto 0);
			  choice : in std_logic_vector (7 downto 0);
			  choice_used : in std_logic
			);
end arbiter_lru_8;

architecture Behavioral of arbiter_lru_8 is
	type lru_array is array (7 downto 0) of std_logic_vector (7 downto 0);
	signal lru : lru_array; --the closest to index 0, the least recently used
begin
	
	--- transition
	process (clk) begin
		if rising_edge(clk) then
			if rst='1' then
				lru(0)<=x"01";
				lru(1)<=x"02";
				lru(2)<=x"04";
				lru(3)<=x"08";
				lru(4)<=x"10";
				lru(5)<=x"20";
				lru(6)<=x"40";
				lru(7)<=x"80";
			elsif choice_used='1' then
				if choice=lru(0) then
					lru(0) <= lru(1);
				end if;
				if choice=lru(0) or choice=lru(1) then
					lru(1) <= lru(2);
				end if;
				if choice=lru(0) or choice=lru(1) or choice=lru(2) then
					lru(2) <= lru(3);
				end if;
				if choice=lru(0) or choice=lru(1) or choice=lru(2) or choice=lru(3) then
					lru(3) <= lru(4);
				end if;
				if choice=lru(0) or choice=lru(1) or choice=lru(2) or choice=lru(3) or choice=lru(4) then
					lru(4) <= lru(5);
				end if;
				if choice=lru(0) or choice=lru(1) or choice=lru(2) or choice=lru(3) or choice=lru(4) or choice=lru(5)then
					lru(5) <= lru(6);
				end if;
				if choice=lru(0) or choice=lru(1) or choice=lru(2) or choice=lru(3) or choice=lru(4) or choice=lru(5) or choice=lru(6) then
					lru(6) <= lru(7);
				end if;
				lru(7) <= choice;
			end if;
		end if;
	end process;
	
	--- generation
	prechoice <=  	lru(0) when empty(0)='0'
				else lru(1) when  empty(1)='0'
				else lru(2) when  empty(2)='0'
				else lru(3) when  empty(3)='0'
				else lru(4) when  empty(4)='0'
				else lru(5) when  empty(5)='0'
				else lru(6) when  empty(6)='0'
				else lru(7) when  empty(7)='0'
				else x"00";
	
end Behavioral;

