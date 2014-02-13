library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity arbiter_lru is
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           empty : in  STD_LOGIC_VECTOR (3 downto 0);
           prechoice : out  STD_LOGIC_VECTOR (3 downto 0);
			  choice : in std_logic_vector (3 downto 0);
			  choice_used : in std_logic
			);
end arbiter_lru;

architecture Behavioral of arbiter_lru is
	type lru_array is array (3 downto 0) of std_logic_vector (3 downto 0);
	signal lru : lru_array; --the closest to index 0, the least recently used
begin
	
	--- transition
	process (clk) begin
		if rising_edge(clk) then
			if rst='1' then
				lru(0)<="0001";
				lru(1)<="0010";
				lru(2)<="0100";
				lru(3)<="1000";
			elsif choice_used='1' then
				if choice=lru(0) then
					lru(0) <= lru(1);
				end if;
				if choice=lru(1) or choice=lru(0) then
					lru(1) <= lru(2);
				end if;
				if choice=lru(2) or choice=lru(1) or choice=lru(0) then
					lru(2) <= lru(3);
				end if;
				lru(3) <= choice;
			end if;
		end if;
	end process;
	
	--- generation
	prechoice <=  	lru(0) when empty(0)='0'
				else lru(1) when  empty(1)='0'
				else lru(2) when  empty(2)='0'
				else lru(3) when  empty(3)='0'
				else "0000";
	
end Behavioral;

