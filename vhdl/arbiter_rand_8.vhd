library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity arbiter_rand_8 is
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           empty : in  STD_LOGIC_VECTOR (7 downto 0);
           prechoice : out  STD_LOGIC_VECTOR (7 downto 0);
			  choice : in std_logic_vector (7 downto 0);
			  choice_used : in std_logic
			);
end arbiter_rand_8;

architecture Behavioral of arbiter_rand_8 is
	signal lfsr : std_logic_vector (31 downto 0);
	signal extracted_bits : std_logic_vector(2 downto 0);
begin
	
	--- transition
	process (clk) begin
		if rising_edge(clk) then
			if rst='1' then
				lfsr <= x"9E4D5DDD"; -- seed
			else
				lfsr(31 downto 1) <= lfsr(30 downto 0);
				lfsr(0) <= lfsr(0) xor (lfsr(26) xor (lfsr(27) xor lfsr(31)));
			end if;
		end if;
	end process;
	
	--- generation
	extracted_bits <= lfsr(15) & lfsr(23) & lfsr(5);
	with extracted_bits select
	prechoice <= 	(0=>(not empty(0)), others=>'0') when "000",
						(1=>(not empty(1)), others=>'0') when "001",
						(2=>(not empty(2)), others=>'0') when "010",
						(3=>(not empty(3)), others=>'0') when "011",
						(4=>(not empty(4)), others=>'0') when "100",
						(5=>(not empty(5)), others=>'0') when "101",
						(6=>(not empty(6)), others=>'0') when "110",
						(7=>(not empty(7)), others=>'0') when others;

end Behavioral;

