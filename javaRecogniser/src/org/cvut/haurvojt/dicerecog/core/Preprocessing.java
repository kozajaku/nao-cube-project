/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.cvut.haurvojt.dicerecog.core;

import java.awt.Point;
import java.util.ArrayList;
import java.util.*;

/**
 *
 * @author haurvojt
 */
public class Preprocessing {

    private static final int MINIMUM_CLUSTER_SIDE = 120;
    
//    public static int[][] smoothen(int[][] pixels) {
//        int x = pixels.length - 1;
//        if (x <= 0) {
//            return null;
//        }
//        int y = pixels[0].length - 1;
//
//        int[][] smoothen = new int[pixels.length][pixels[0].length];
//
//        for (int i = 1; i < x; i++) {
//
//            for (int j = 1; j < y; j++) {
//
//                int[] neighbors = {pixels[i - 1][j], pixels[i + 1][j], pixels[i][j - 1], pixels[i][j + 1]};
//                int[] colorFilters = {0, 8, 16};
//                int newColor = 0xff000000;
//                for (int cF : colorFilters) {
//
//                    int part = (pixels[i][j] >> cF) & 0xff;
//                    int temp = part;
//                    for (int n : neighbors) {
//                        temp += ((n >> cF) & 0xff);
//                        temp += part;
//                    }
//                    temp /= (neighbors.length * 2 + 1);
//                    if (temp < 0) {
//                        temp = 0;
//                    }
//                    if (temp > 0xff) {
//                        newColor += 0xff << cF;
//                    } else {
//                        newColor += (temp << cF);
//
//                    }
//
//                }
//                smoothen[i][j] = newColor;
//
//            }
//
//        }
//        return smoothen;
//
//    }

    public static List<SubpictureBundle> findPossibleDice(int[][] pixels, int height, int width) {
        List<DiceCluster> processingClusters = new ArrayList<>();
        List<DiceCluster> finishedClusters = new ArrayList<>();

        for (int h = 0; h < height; h++) {

            // must be list, ordering of items is crucial
            List<Point> unassigned = new ArrayList<>();

            for (int w = 0; w < width; w++) {

                if (!Common.isPixelRed(pixels[h][w])) {
                    // this pixel is not of proper color, will drop is 
                    continue;
                }

                Point point = new Point(h, w);

                boolean addedTo = assignPointToDiceCluster(point, processingClusters);

                if (!addedTo) {
                    unassigned.add(point);
                }

            }

            // backward addition that ensures that even point that were 
            for (int i = unassigned.size() - 1; i >= 0; i--) {
                if (!assignPointToDiceCluster(unassigned.get(i), processingClusters)) {
                    processingClusters.add(new DiceCluster(unassigned.get(i)));
                }
            }

            // this will update shift to next line in all clusters and withdraw those that are already finished (there are
            // no points in current line and thus it is impossible to add points in next line)
            Iterator<DiceCluster> iterator = processingClusters.iterator();

            while (iterator.hasNext()) {
                DiceCluster dc = iterator.next();
                if (dc.moveToNextLine()) {
                    finishedClusters.add(dc);
                    iterator.remove();
                }

            }

        }


        List<SubpictureBundle> subPictures = new ArrayList<>();
        for (DiceCluster dc : finishedClusters) {

//            System.err.println("DC height: " + dc.topMost + " to " + dc.bottomMost + " width: " + dc.leftMost + " to " + dc.rightMost);

            if (dc.getHeight() < MINIMUM_CLUSTER_SIDE || dc.getWidth() < MINIMUM_CLUSTER_SIDE) {

                continue;
                // we dont want so small cluster that there cannot even be 3 dots possibly next to each other
            }

            int[][] subPicture = new int[dc.bottomMost - dc.topMost + 1][dc.rightMost - dc.leftMost + 1];
            // retrieving data from pixels to subpicture
            for (int i = dc.topMost; i <= dc.bottomMost; i++) {


                subPicture[i - dc.topMost] = Arrays.copyOfRange(pixels[i], dc.leftMost, dc.rightMost + 1);
                for (int j = dc.leftMost; j < dc.lineStarts.get(i); j++) {                    
                    subPicture[i - dc.topMost][j - dc.leftMost] = 0xffffffff;
                }
                for (int j = dc.rightMost; j > dc.lineEnds.get(i); j--) {                 
                    subPicture[i - dc.topMost][j - dc.leftMost] = 0xffffffff;
                }

            }
            subPictures.add(new SubpictureBundle(subPicture, dc.topMost));
        }
        return subPictures;

    }

//    public static int[][] enlarge(int[][] image, int n) {
//
//        int h = n * image.length;
//        if (h < 1) {
//            throw new IllegalArgumentException("Height must be positive");
//        }
//        int w = n * image[0].length;
//
//        int[][] enlarged = new int[h][w];
//
//        for (int y = 0; y < h; y++) {
//            for (int x = 0; x < w; x++) {
//                enlarged[y][x] = image[y / n][x / n];
//            }
//        }
//
//        return enlarged;
//    }

    private static boolean assignPointToDiceCluster(Point point, List<DiceCluster> diceClusters) {
        DiceCluster addedTo = null;
        for (DiceCluster dc : diceClusters) {

            if (dc.addPoint(point)) {
                // merge two DiceClusters if the both neighbor with one pixel
                if (addedTo != null) {
                    dc.merge(addedTo);
                    diceClusters.remove(addedTo);
                    break;
                } else {
                    addedTo = dc;
                }
            }
        }
        return !(addedTo == null);
    }

    private static boolean nextToEachOther(Point a, Point b) {
        return (Math.abs(a.x - b.x) + Math.abs(a.y - b.y)) == 1;
    }

    private static class DiceCluster {

        int topMost, leftMost, rightMost, bottomMost;

        Map<Integer, Integer> lineStarts;
        Map<Integer, Integer> lineEnds;
        Set<Point> pointsInPreviousLine;
        Set<Point> pointsInCurrentLine;

        public DiceCluster(Point first) {
            pointsInCurrentLine = new HashSet<>();
            pointsInPreviousLine = new HashSet<>();
            lineEnds = new HashMap<>();
            lineStarts = new HashMap<>();

            pointsInCurrentLine.add(first);
            topMost = bottomMost = first.x;
            leftMost = rightMost = first.y;

            lineEnds.put(first.x, first.y);
            lineStarts.put(first.x, first.y);
        }

        /**
         *
         * @param point
         * @return true if added successfully
         */
        public boolean addPoint(Point point) {
            for (Point my : pointsInPreviousLine) {
                if (nextToEachOther(point, my)) {
                    pointsInCurrentLine.add(point);
                    updateMostTrackers(point);
                    return true;
                }
            }

            for (Point my : pointsInCurrentLine) {
                if (nextToEachOther(point, my)) {
                    pointsInCurrentLine.add(point);
                    updateMostTrackers(point);
                    return true;
                }
            }

            return false;
        }

        private void updateMostTrackers(Point p) {
            // global markers update 
            if (p.y < leftMost) {
                leftMost = p.y;
            }
            if (p.y > rightMost) {
                rightMost = p.y;
            }
            if (p.x > bottomMost) {
                bottomMost = p.x;
            }

            if(!lineStarts.containsKey(p.x)){
                lineStarts.put(p.x, p.y);
            }
            if(!lineEnds.containsKey(p.x)){
                lineEnds.put(p.x, p.y);
            }
            // line markers update
            if (lineStarts.get(p.x) > p.y) {
                lineStarts.put(p.x, p.y);
            }
            if (lineEnds.get(p.x) < p.y) {
                lineEnds.put(p.x, p.y);
            }
        }

        /**
         *
         * @return true if there were no points in current line, meaning, that
         * this cluster is finished
         */
        public boolean moveToNextLine() {
            if (pointsInCurrentLine.isEmpty()) {

                return true;
            }

            pointsInPreviousLine = pointsInCurrentLine;
            pointsInCurrentLine = new HashSet<>();
            return false;
        }

        public void merge(DiceCluster dc) {
            this.pointsInPreviousLine.addAll(dc.pointsInPreviousLine);
            this.pointsInCurrentLine.addAll(dc.pointsInCurrentLine);
            
            topMost = Math.min(topMost, dc.topMost);
            bottomMost = Math.max(bottomMost, dc.bottomMost);
            leftMost = Math.min(leftMost, dc.leftMost);
            rightMost = Math.max(rightMost, dc.rightMost);
            
            // merge lineStarts and lineEnds
            for(Integer key : dc.lineStarts.keySet()){
                if(lineStarts.containsKey(key)){
                    lineStarts.put(key, Math.min( dc.lineStarts.get(key), lineStarts.get(key) ));
                }
                else{
                    lineStarts.put(key, dc.lineStarts.get(key));
                }
            }
            for(Integer key : dc.lineEnds.keySet()){
                if(lineEnds.containsKey(key)){
                    lineEnds.put(key, Math.max( dc.lineEnds.get(key), lineEnds.get(key) ));
                }
                else{
                    lineEnds.put(key, dc.lineEnds.get(key));
                }
            }
            
        }

        public int getHeight() {
            return bottomMost - topMost + 1;
        }

        public int getWidth() {
            return rightMost - leftMost + 1;
        }

    }

}
