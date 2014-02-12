library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity arbiter_fixed is
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           empty : in  STD_LOGIC_VECTOR (3 downto 0);
           prechoice : out  STD_LOGIC_VECTOR (3 downto 0);
			  choice : in std_logic_vector (3 downto 0);
			  choice_used : in std_logic
			);
end arbiter_fixed;

architecture Behavioral of arbiter_fixed is
	signal choice_tmp : std_logic_vector (3 downto 0);
begin
	
	choice_tmp(3) <=  not empty(3);
	choice_tmp(2) <=  empty(3) and not empty(2);
	choice_tmp(1) <=  empty(3) and empty(2) and not empty(1);
	choice_tmp(0) <=  empty(3) and empty(2) and empty(1) and not empty(0);
	
	prechoice <= choice_tmp;

end Behavioral;

