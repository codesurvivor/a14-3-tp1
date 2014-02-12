library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity arbiter_fixed_8 is
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           empty : in  STD_LOGIC_VECTOR (7 downto 0);
           prechoice : out  STD_LOGIC_VECTOR (7 downto 0);
			  choice : in std_logic_vector (7 downto 0);
			  choice_used : in std_logic
			);
end arbiter_fixed_8;

architecture Behavioral of arbiter_fixed_8 is
	signal choice_tmp : std_logic_vector (7 downto 0);
begin
	
	choice_tmp(7) <=  not empty(7);
	choice_tmp(6) <=  empty(7) and not empty(6);
	choice_tmp(5) <=  empty(7) and empty(6) and not empty(5);
	choice_tmp(4) <=  empty(7) and empty(6) and empty(5) and not empty(4);
	choice_tmp(3) <=  empty(7) and empty(6) and empty(5) and empty(4) and not empty(3);
	choice_tmp(2) <=  empty(7) and empty(6) and empty(5) and empty(4) and empty(3) and not empty(2);
	choice_tmp(1) <=  empty(7) and empty(6) and empty(5) and empty(4) and empty(3) and empty(2) and not empty(1);
	choice_tmp(0) <=  empty(7) and empty(6) and empty(5) and empty(4) and empty(3) and empty(2) and empty(1)and not empty(0);
	
	prechoice <= choice_tmp;

end Behavioral;

