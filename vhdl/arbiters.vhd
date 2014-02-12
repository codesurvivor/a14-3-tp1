library IEEE;
use IEEE.std_logic_1164.all;
 
package arbiters is
 
component arbiter_round is
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           empty : in  STD_LOGIC_VECTOR (3 downto 0);
           prechoice : out  STD_LOGIC_VECTOR (3 downto 0);
			  choice : in std_logic_vector (3 downto 0);
			  choice_used : in std_logic
			);
end component arbiter_round;

component arbiter_rand is
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           empty : in  STD_LOGIC_VECTOR (3 downto 0);
           prechoice : out  STD_LOGIC_VECTOR (3 downto 0);
			  choice : in std_logic_vector (3 downto 0);
			  choice_used : in std_logic
			);
end component arbiter_rand;

component arbiter_lru is
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           empty : in  STD_LOGIC_VECTOR (3 downto 0);
           prechoice : out  STD_LOGIC_VECTOR (3 downto 0);
			  choice : in std_logic_vector (3 downto 0);
			  choice_used : in std_logic
			);
end component arbiter_lru;

component arbiter_fixed is
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           empty : in  STD_LOGIC_VECTOR (3 downto 0);
           prechoice : out  STD_LOGIC_VECTOR (3 downto 0);
			  choice : in std_logic_vector (3 downto 0);
			  choice_used : in std_logic
			);
end component arbiter_fixed;

end arbiters;