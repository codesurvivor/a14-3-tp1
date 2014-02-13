library IEEE;
use IEEE.Std_logic_1164.all;
use IEEE.Numeric_Std.all;

entity tb_traffic is
end;

architecture bench of tb_traffic is

  component traffic_acceptor
      Port ( clk : in  STD_LOGIC;
  			  rst : in std_logic;
             din : in std_logic_vector(39 downto 0);
  			  din_req : in std_logic;
  			  din_ack : out std_logic
  			);
  end component;
  
  component traffic_generator
	 Generic ( 
				id : std_logic_vector (4 downto 0);
				target : std_logic_vector (1 downto 0);
				cycles_per_msg : integer);
    Port ( clk : in  STD_LOGIC;
			  rst : in std_logic;
           dout : out std_logic_vector(39 downto 0);
			  dout_req : out std_logic;
			  dout_ack : in std_logic
			);
	end component traffic_generator;

  signal clk: STD_LOGIC;
  signal rst: std_logic;
  signal din: std_logic_vector(39 downto 0);
  signal din_req: std_logic;
  signal din_ack: std_logic;

  constant clock_period: time := 10 ns;
  signal stop_the_clock: boolean := false;

begin

  uut: traffic_acceptor port map ( clk     => clk,
                                   rst     => rst,
                                   din     => din,
                                   din_req => din_req,
                                   din_ack => din_ack );
											  
	tg0 : traffic_generator generic map ( id             => "00001",
                                       target         => "01",
                                       cycles_per_msg =>  2)
                            port map ( clk            => clk,
                                       rst            => rst,
                                       dout           => din,
                                       dout_req       => din_req,
                                       dout_ack       => din_ack );

  stimulus: process
  begin
  
    -- Put initialisation code here
		rst <= '1';
      wait for clock_period*10;
		rst <= '0';

    -- Put test bench stimulus code here
	 wait for clock_period*300;

    stop_the_clock <= true;
    wait;
  end process;

  clocking: process
  begin
    while not stop_the_clock loop
      clk <= '0', '1' after clock_period / 2;
      wait for clock_period;
    end loop;
    wait;
  end process;

end;