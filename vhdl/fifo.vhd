library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.all;

entity fifo is
	generic (nb_stages : integer);
    Port ( 
			clk : in std_logic;
			rst : in std_logic;
			
			din : in std_logic_vector(39 downto 0);
			wr_req : in std_logic;
			wr_ack : out std_logic;
			
			dout : out std_logic_vector(39 downto 0);
			rd_ack : in std_logic;
			rd_req : out std_logic
		);
end fifo;

architecture Behavioral of fifo is
	type datas_type is array (nb_stages downto 0) of std_logic_vector(39 downto 0);
	signal reqs : std_logic_vector(nb_stages downto 0);
	signal acks : std_logic_vector(nb_stages downto 0);
	signal datas : datas_type;
begin
	GEN_REG: 
   for i in 0 to nb_stages-1 generate
      fifo_stage_X : entity fifo_stage port map
        (clk => clk,
			rst => rst,
			
			din => datas(i),
			dout => datas(i+1),
			
			pred_req => reqs(i),
			pred_ack => acks(i),
			succ_req => reqs(i+1),
			succ_ack => acks(i+1)
		);
   end generate GEN_REG;
	
	reqs(0) <= wr_req;
	wr_ack <= acks(0);
	
	rd_req <= reqs(nb_stages);
	acks(nb_stages) <= rd_ack;
	
	dout <= datas(nb_stages);
	datas(0) <= din;
	
end Behavioral;

