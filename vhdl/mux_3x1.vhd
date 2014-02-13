library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity mux_3x1 is
    Port ( in0 : in  STD_LOGIC_VECTOR (39 downto 0);
           in1 : in  STD_LOGIC_VECTOR (39 downto 0);
           in2 : in  STD_LOGIC_VECTOR (39 downto 0);
           out0 : out  STD_LOGIC_VECTOR (39 downto 0);
			  
			  sel : in STD_LOGIC_VECTOR (2 downto 0)
		);
end mux_3x1;

architecture Behavioral of mux_3x1 is

begin
	with sel select
		out0 <= 	in0 when "001",
					in1 when "010",
					in2 when others;

end Behavioral;

