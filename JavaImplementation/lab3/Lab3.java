/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab3;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Map;
import java.util.Scanner;

/**
 *
 * @author Ryan
 */
public class Lab3 {

    /**
     * @param args contains the name of the file to read in (index 0)
     */
    public static void main(String[] args) {
        int t = 6; // iterations to run
        Scanner scan = new Scanner(System.in);
        System.out.println("Enter file name: ");
        String file = scan.nextLine();
        CSVParser parser = new CSVParser(file);
        long startParse = System.currentTimeMillis();
        parser.readAllLines();
        System.out.println(System.currentTimeMillis() - startParse 
         + " milliseconds to parse");

        long startRank = System.currentTimeMillis();
        /*for (String s : parser.foundNodes) {
            parser.singleRank(parser.nodes.get(s), t, true);
        }*/
        parser.pageRank();
        System.out.println(System.currentTimeMillis() - startRank
         + " milliseconds to rank pages");
        //System.out.println("Ran " + t + " iterations");
        
        // sort nodes for printing results
        ArrayList<Node> n = new ArrayList<Node>();
        for (Map.Entry<String,Node> edge : parser.nodes.entrySet()) {
            n.add(edge.getValue());
        }
        Collections.sort(n);
        int j = 1;
        for (int i = n.size() - 1; i >= 0; i--) {
            System.out.println(j++ + ": " + n.get(i).identifier + 
             " with rank " + n.get(i).rank);
        }
    }
    
}
