import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.HashMap;

public class NASMtoString {

	public static void main(String[] args) throws IOException {
        // Path to the file
        String filePath = "src/input";

        try {
            // Open the file
            File file = new File(filePath);
            FileInputStream fis = new FileInputStream(file);

            int byteValue; // Holds each byte read
            int address = 0; // Memory address for formatting

            while ((byteValue = fis.read()) != -1) {
                String binaryString = String.format("%8s", Integer.toBinaryString(byteValue & 0xFF)).replace(' ', '0');

                System.out.print(binaryString + " ");

                address++;

                if (address % 2 == 0) {
                    System.out.println();
                }
            }

            System.out.println("\nDone reading file.");
            fis.close();
        } catch (IOException e) {
            System.err.println("An error occurred while reading the file: " + e.getMessage());
        }
		
		
		}
		
	}
