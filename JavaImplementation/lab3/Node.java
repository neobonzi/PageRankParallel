/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab3;

import java.util.ArrayList;
import java.util.HashMap;

/**
 *
 * @author Ryan + Greg
 */
public class Node implements Comparable<Node>{
    public String identifier;
    public HashMap<String, Integer> edgeWeights = new HashMap<String, Integer>();
    public ArrayList<Node> referencedBy = new ArrayList<Node>();
    public String extraInfo;
    int inDegree;
    double outDegree;
    double rank = 0;
    
    public Node(String id) {
        identifier = id;
        inDegree = 0;
        outDegree = 0;
    }
    
    @Override
    public int compareTo(Node n) {
        if (this.rank < n.rank)
            return -1;
        else if (this.rank == n.rank)
            return 0;
        else
            return 1;
    }
    
}
