/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab3;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.StringTokenizer;

/**
 *
 * @author Ryan + Greg
 */
public class CSVParser {
    Scanner fileScanner;
    String fileName;
    ArrayList<String> foundNodes;
    HashMap<String, Node> nodes;
    
    public CSVParser(String file) {
        try {
           fileName = file;
           fileScanner = new Scanner(new File(file));
           foundNodes = new ArrayList<String>();
           nodes = new HashMap<String,Node>();
        }
        catch (Exception e) {
            System.err.println("File not found");
        }
    }
    
    private void readLine() {
        String line = fileScanner.nextLine();
        StringTokenizer tok = new StringTokenizer(line, ",");
        String node1, score1, node2, score2, extra = "";
        int val1, val2;
        Node teamNode1, teamNode2;

        node1 = tok.nextToken().trim();
        score1 = tok.nextToken();
        if (score1.contains("\"")) {
            score1.replaceAll("\"", "");
        }
        node2 = tok.nextToken().trim();
        score2 = tok.nextToken();
        if (score2.contains("\"")) {
            score2.replaceAll("\"", "");
        }
        if (tok.hasMoreTokens())
            extra = tok.nextToken();
        
        if (!foundNodes.contains(node1)) {
            foundNodes.add(node1);
            teamNode1 = new Node(node1);
        }
        else {
            teamNode1 = nodes.get(node1);
        }
        
        if (!foundNodes.contains(node2)) {
            foundNodes.add(node2);
            teamNode2 = new Node(node2);
        }
        else {
            teamNode2 = nodes.get(node2);
        }
        
        val1 = Integer.parseInt(score1.trim());
        val2 = Integer.parseInt(score2.trim());
        
        /* both the following cases will run if values are equal
           which signifies an undirected graph
        */
        // if value 2 is greater, indicates edge from node1 to node2
        if (val2 >= val1) {
            teamNode1.edgeWeights.put(node2, val2);
            teamNode2.inDegree++;
            teamNode2.referencedBy.add(teamNode1);
            teamNode1.outDegree++;
        }
        // if value 1 is greater, indicates edge from node2 to node1
        if (val1 >= val2) {
            teamNode2.edgeWeights.put(node1, val1);
            teamNode1.inDegree++;
            teamNode1.referencedBy.add(teamNode2);
            teamNode2.outDegree++;
        }
        
        nodes.put(node1,teamNode1);
        nodes.put(node2,teamNode2);
    }
    
    public void readAllLines() {
        while (fileScanner.hasNextLine()) {
            readLine();
        }
    }
    
    public void pageRank() {
        int i;
        double v = foundNodes.size(), d = 0.9, link, jump;
        Node cur, next;
        
        // sets initial pageRank
        for (i = 0; i < v; i++) {
            cur = nodes.get(foundNodes.get(i));
            cur.rank = 1.0/v;
            //System.out.println(foundNodes.get(i) + " starting with " + cur.rank);
            nodes.put(foundNodes.get(i), cur);
        }
        
        // iterate trhough other calls of pageRank
        for (int j = 0; j < 1000; j++) {
            for (i = 0; i < v; i++) {
                cur = nodes.get(foundNodes.get(i));
                jump = (1.0-d)*(1.0/v)+d;
                link = 0;
                
                //for (Map.Entry<String, Integer> edge: cur.edgeWeights.entrySet()) {
                for (Node n : cur.referencedBy) {
                    //next = nodes.get(edge.getKey());
                    link += (double)(1.0/n.outDegree) * n.rank;
                }
                
                //cur.rank = jump * link;
                cur.rank = (1.0-d)*(1.0/v)+d*link;
            }
        }
    }
    /**
     * 
     * @param n node to be ranked
     * @param t iteration of the method
     * @param update only true for original call, saves rank within the node
     * @return the value of the node
     */
    public double singleRank(Node n, int t, boolean update) {
        double v = foundNodes.size(), d = 0.85;
        double sum = 0;
        if (t == 0) {
            //n.rank = 1.0/v;
            return 1.0/v;
        }
        else {
            // find pageRank of all nodes that reference current node
            for (Node next : n.referencedBy) {
                //Node next = nodes.get(s); 
                sum += (1/next.outDegree) * singleRank(next, t-1, false);
            }

            if (update) {
                n.rank = (1.0-d)*(1.0/v)+d*sum;
                nodes.put(n.identifier, n);
            }
                
            return (1.0-d)*(1.0/v)+d*sum;
        }
        //nodes.put(n.identifier, n);
        //return n.rank;
    }
    
}
