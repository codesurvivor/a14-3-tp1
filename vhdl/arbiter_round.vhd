library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity arbiter_round is
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           empty : in  STD_LOGIC_VECTOR (3 downto 0);
           prechoice : out  STD_LOGIC_VECTOR (3 downto 0);
			  choice : in std_logic_vector (3 downto 0);
			  choice_used : in std_logic
			);
end arbiter_round;

architecture Behavioral of arbiter_round is
	signal first : std_logic_vector (3 downto 0);
begin
	
	--- transition
	process (clk) begin
		if rising_edge(clk) then
			if rst='1' then
				first <= "0001";
			elsif choice_used='1' then
				first(3 downto 1) <= choice(2 downto 0);
				first(0) <= choice(3);
			end if;
		end if;
	end process;
	
	--- generation
	prechoice(3) <= (not empty(3)) and (first(3) 
														or (first(2) and empty(2))
														or (first(1) and empty(2) and empty(1))
														or (first(0) and empty(2) and empty(1) and empty(0)));
	prechoice(2) <= (not empty(2)) and (first(2) 
														or (first(1) and empty(1))
														or (first(0) and empty(1) and empty(0))
														or (first(3) and empty(1) and empty(0) and empty(3)));
	prechoice(1) <= (not empty(1)) and (first(1) 
														or (first(0) and empty(0))
														or (first(3) and empty(0) and empty(3))
														or (first(2) and empty(0) and empty(3) and empty(2)));
	prechoice(0) <= (not empty(0)) and (first(0) 
														or (first(3) and empty(3))
														or (first(2) and empty(3) and empty(2))
														or (first(1) and empty(3) and empty(2) and empty(1)));

end Behavioral;

