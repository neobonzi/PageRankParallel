import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

/**
 * Created by Greg + Ryan on 10/13/15.
 */
public class SNAPParser {

    static HashMap<Integer,SNAPNode> nodeList = new HashMap<Integer, SNAPNode>();
    static boolean directed = false;
    static long startTime = 0;

    public static void main(String args[]){


        parseData();
        long readTime = System.currentTimeMillis();
//        if(!directed)
//            undirectedRankPages();
//        else
        directedRankPages();
        long endTime = System.currentTimeMillis();

        System.out.println("Time taken to read in milliseconds is: " + (readTime-startTime));
        System.out.println("Time taken to rank pages in milliseconds is: " + (endTime-readTime));

    }

    private static void parseData(){
        Scanner in = new Scanner(System.in);
        Scanner parser = null;
        File file;
        System.out.println("Enter File Name");
        String fileName = in.next();

        try {
            file = new File(fileName);
            parser = new Scanner(file);
        }
        catch(FileNotFoundException e){
            e.printStackTrace();
        }

        int fromNode;
        int toNode;

        System.out.println("Graph Directed or Undirected? (D/U)");
        String nextIn = in.next();
        if(nextIn.equals("D") || nextIn.equals("d"))
            directed = true;
        else
            directed = false;

        startTime = System.currentTimeMillis();
        while(parser.hasNextLine()){
            //System.out.println("Inside the while");
            String nextLine = parser.nextLine();
            if(nextLine.charAt(0) != '#') {
                Scanner line = new Scanner(nextLine);
                fromNode = line.nextInt();
                SNAPNode update;
                if (nodeList.containsKey(fromNode))
                    update = nodeList.get(fromNode);
                else
                    update = new SNAPNode(fromNode);

                toNode = line.nextInt();
                //System.out.println(toNode);
                int weight = 0;
                if (line.hasNextInt())
                    weight = line.nextInt();

                update.edges++;

                SNAPNode to;
                if(!nodeList.containsKey(toNode))
                    to = new SNAPNode(toNode);
                else
                    to = nodeList.get(toNode);

                if(weight == -1) {
                    update.inEdges.add(toNode);
                    to.edges++;
                }
                else
                    to.inEdges.add(fromNode);


                nodeList.put(fromNode, update);
                nodeList.put(toNode, to);
            }
        }
    }

//    private static void undirectedRankPages(){
//        System.out.println("Inside page ranks");
//        int i, v = nodeList.size(), r;
//        double d = 1.05, linkProb, jumpProb;
//        SNAPNode other;
//        SNAPNode next;
//        int size = (int)Math.sqrt(nodeList.size())/2 ;
//        ArrayList<Double> topScores = new ArrayList<Double>(size);
//        ArrayList<SNAPNode> topNodes = new ArrayList<SNAPNode>(size);
//
//        for(i = 0; i < size; i++){
//            topScores.add(-1000.0);
//            topNodes.add(new SNAPNode(-1000.0));
//        }
//
//        for(Map.Entry<Integer, SNAPNode> entry : nodeList.entrySet()) {
//            next = entry.getValue();
//            next.pageRank = 1.0 / nodeList.size();
//            nodeList.put(next.identifier, next);
//        }
//
//        for(r = 0; r < 100; r++){
//            for(Map.Entry<Integer, SNAPNode> entry : nodeList.entrySet()){
//                next = entry.getValue();
//                jumpProb = (1-d) * (1/v) + d;
//                linkProb = 0.0;
//                for(Map.Entry<Integer, Integer> edge : next.edges.entrySet()){
//                    other = nodeList.get(edge.getKey());
//                    linkProb += (1/other.edges.size()) * other.pageRank;
//                }
//                next.pageRank = jumpProb * linkProb;
//                nodeList.put(next.identifier, next);
//            }
//        }
//
//        for(Map.Entry<Integer, SNAPNode> entry : nodeList.entrySet()){
//
//            SNAPNode node = entry.getValue();
//            if(topScores.contains(-1000.0)) {
//                topScores.set(size - 1, node.pageRank);
//                topNodes.set(size -1, node);
//            }
//            else {
//                if (topScores.get(size - 1) < node.pageRank) {
//                    topScores.set(size - 1, node.pageRank);
//                    topNodes.set(size - 1, node);
//                }
//            }
//            Collections.sort(topScores);
//            Collections.sort(topNodes);
//        }
//
//        System.out.println("Top nodes by page rank.");
//        for(i = 0; i<topNodes.size(); i++){
//            System.out.println("Node #: " + topNodes.get(i).identifier+ " with score: " + topNodes.get(i).pageRank);
//        }
//    }

    private static void directedRankPages(){
        /**
         * For wiki, d = 1.40
         *  gnutella d = 2.18
         *  soc-sign d = 1.007
         *
         */
        int i, v = nodeList.size(), r;
        double d = 0.75, linkProb, jumpProb;
        SNAPNode other;
        SNAPNode next;
        int size = 120 ;
        ArrayList<Double> topScores = new ArrayList<Double>(size);
        ArrayList<SNAPNode> topNodes = new ArrayList<SNAPNode>(size);

        for(i = 0; i < size; i++){
            topScores.add(-1000.0);
            topNodes.add(new SNAPNode(-1000.0));
        }

        for(Map.Entry<Integer, SNAPNode> entry : nodeList.entrySet()) {
            next = entry.getValue();
            next.pageRank = 1.0/nodeList.size();
            nodeList.put(next.identifier, next);
        }

        for(r = 0; r < 10; r++){
            for(Map.Entry<Integer, SNAPNode> entry : nodeList.entrySet()){
                next = entry.getValue();
                jumpProb = (1.0-d) * (1.0/v);
                linkProb = 0.0;
                for(Integer edge : next.inEdges){
                    other = nodeList.get(edge);
                    linkProb += (1.0/other.edges) * other.pageRank;
                }
                next.pageRank = (jumpProb + (d * linkProb));
                nodeList.put(next.identifier, next);
            }
        }

        for(Map.Entry<Integer, SNAPNode> entry : nodeList.entrySet()){

            SNAPNode node = entry.getValue();
            if(topScores.contains(-1000.0)) {
                topScores.set(size - 1, node.pageRank);
                topNodes.set(size -1, node);
            }
            else {
                if (topScores.get(size - 1) < node.pageRank) {
                    topScores.set(size - 1, node.pageRank);
                    topNodes.set(size - 1, node);
                }
            }
            Collections.sort(topScores);
            Collections.sort(topNodes);
        }

        System.out.println("Top nodes by page rank:");
        for(i = 0; i<topNodes.size(); i++){
            System.out.println("Node #: " + topNodes.get(i).identifier+ " with score: " + topNodes.get(i).pageRank *100);
        }
    }
}
