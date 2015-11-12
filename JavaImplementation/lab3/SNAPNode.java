import java.util.ArrayList;
import java.util.HashMap;

/**
 * Created by Greg + Ryan on 10/13/15.
 */
public class SNAPNode implements Comparable<SNAPNode>{

    int identifier;
    public double pageRank = 1.0;
    int edges = 0;
    public ArrayList<Integer> inEdges = new ArrayList<Integer>(); //Used only in directed graphs

    public SNAPNode(int id){
        identifier = id;
//        pageRanks = new ArrayList<Double>();
    }
    public SNAPNode(double rank){
        pageRank = rank;
    }

    public int compareTo(SNAPNode other){
        if(other.pageRank < pageRank)
            return -1;
        else
            return 1;
    }


}
