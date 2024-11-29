import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;

public class Homework1 {

	public static void main(String[] args) throws IOException {
		HashMap<String,String> mapW0 = new HashMap<String, String>();
		HashMap<String,String> mapW1 = new HashMap<String, String>();
		
		mapW0.put("000", "al");
		mapW0.put("001", "cl");
		mapW0.put("010", "dl");
		mapW0.put("011", "bl");
		mapW0.put("100", "ah");
        mapW0.put("101", "ch");
        mapW0.put("110", "dh");
        mapW0.put("111", "bh");
        
        mapW1.put("000", "ax");
        mapW1.put("001", "cx");
        mapW1.put("010", "dx");
        mapW1.put("011", "bx");
        mapW1.put("100", "sp");
        mapW1.put("101", "bp");
        mapW1.put("110", "si");
        mapW1.put("111", "di");
        
        BufferedReader br = new BufferedReader(new FileReader("src/input.txt"));
        String line;
        System.out.println("bits 16");
        System.out.println("");
        
        while((line = br.readLine()) != null) {
        	String[] instructions = line.split(" ");
        	boolean wideBit = instructions[0].charAt(7) == '1'  ? true : false;
        	boolean destinationBit = instructions[0].charAt(6) == '1' ? true : false;
        	HashMap<String, String> map = wideBit ? mapW1 : mapW0;
        	
        	String reg1 = map.get(instructions[1].substring(2, 5));
        	String reg2 = map.get(instructions[1].substring(5, 8));
        	
        	String t1;
        	String t2;
        	
        	t1 = destinationBit ? reg1 : reg2;
        	t2 = destinationBit ? reg2 : reg1;
        	
        	System.out.println("mov " + t1 + ", " + t2);
        }
		
	}

}
