/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.cvut.haurvojt.dicerecog.core;

import java.awt.Point;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 *
 * @author haurvojt
 */
public class ClusterProcessing {
    
    private static List<List<Point>> clustersInEllipse (List<List<Point>> clusters, Ellipse ellipse){
        List<List<Point>> correctPoints = new ArrayList<>();
        for(List<Point> cluster : clusters){
            if(ellipse.isPointInside(clusterCenter(cluster))){
                correctPoints.add(cluster);
            }
        }
        return correctPoints;
    }

    private static Point clusterCenter(List<Point> cluster){
        if(cluster.isEmpty()){
            throw new IllegalArgumentException("Cluster must not be empty");
        }
        int x = 0;
        int y = 0;
        for(Point p : cluster){
            x += p.x;
            y += p.y;
        }
        
        return new Point(x / cluster.size(), y / cluster.size());
    } 

    public static List<Point> advancedProcessCluster(List<List<Point>> clusters, int[][] pixels, Ellipse ellipse) {
        
        if(clusters.isEmpty()){
            return new ArrayList<>();
        }
        
        // removing small Clusters
        // only consider clusters in the ellipse for maximum
        Iterator<List<Point>> iterator = clustersInEllipse(clusters, ellipse).iterator();
        int maxSize = 0;
        while(iterator.hasNext()){
            List<Point> cluster = iterator.next();
            if(maxSize < cluster.size()){
                maxSize = cluster.size();
            }
        }
        
        // now consider all clusters as usual
        iterator = clusters.iterator();
        while(iterator.hasNext()){
            List<Point> cluster = iterator.next();
            if(cluster.size() < maxSize / 10){
                iterator.remove();
            }
        }

        System.out.println("Cluster count: " + clusters.size());
        int recolor = 0x003344ff;
        int add = 0x11000000;
        List<Point> dicePoints = new ArrayList<>();

        int maxForCluster = 50;

        int i = 0;
        for (List<Point> cluster : clusters) {
            

            List<Point> copyCluster = new ArrayList<>(cluster);
            for (Point point : cluster) {

                if (hasRedNeighbors(point, pixels)) {
                    pixels[point.x][point.y] = recolor + add;
                    copyCluster.remove(point);
                }
            }


            List<Point> copyCopyCluster = new ArrayList<>(copyCluster);
            for (int unused = 0; unused < cluster.size(); unused++) {
                if(copyCluster.isEmpty()){
                    break;
                }
                Map<Point, Integer> changes = new HashMap<>();
                for (Point point : copyCluster) {
                    int recolors = recoloredNeighbors(point, pixels);
                    if (recolors > 0) {
                        changes.put(point, recolors + 1);
                        copyCopyCluster.remove(point);
                        maxForCluster = recolors + 1;
                    }
                }
                for (Point point : changes.keySet()) {
                    
                    if(changes.get(point) >= 0xe){
                        pixels[point.x][point.y] = recolor + add * (changes.get(point) - 1);
                    }
                    else{
                        pixels[point.x][point.y] = recolor + add * changes.get(point);
                    }
                }
                copyCluster = new ArrayList<>(copyCopyCluster);
            }
            


            // sub cluster creation
            Set<Point> peaks = new HashSet<>();
            for (Point p : cluster) {
                if ((pixels[p.x][p.y] & 0x00ffffff) == recolor
                        && ((pixels[p.x][p.y] & 0x0f000000) >> 24) >= (maxForCluster - 1)) {
                    peaks.add(p);
                }
            }

            while (!peaks.isEmpty()) {
                List<Point> localPeak = new ArrayList<>();
                Point first = peaks.iterator().next();
                peaks.remove(first);
                localPeak.add(first);
                Common.recursiveCluster(peaks, first, localPeak);
                if (!localPeak.isEmpty()) {
                    int x = 0, y = 0;
                    for (Point p : localPeak) {
                        x += p.x;
                        y += p.y;
                    }
                    dicePoints.add(new Point(x / localPeak.size(), y / localPeak.size()));
                }

            }

            
            for (Point p : dicePoints) {
                pixels[p.x][p.y] = 0xff33ff66;
            }
            
        }
        
        System.out.println("There are " + dicePoints.size() + " points");
        return dicePoints;
    }

    private static boolean hasRedNeighbors(Point p, int[][] pixels) {
        int[][] vectors = Common.getSurrounding();

        int count = 0;
        for (int[] v : vectors) {
            int x = p.x + v[0];
            if (x < 0 || x >= pixels.length) {
                continue;
            }
            int y = p.y + v[1];
            if (y < 0 || y >= pixels[x].length) {
                continue;
            }
            if (Common.isPixelRed(pixels[x][y])) {
                return true;
            }
        }
        return false;

    }

    private static int recoloredNeighbors(Point p, int[][] pixels) {
        int recolor = 0x003344ff;

        int[][] vectors = Common.getSurrounding();

        int count = 0;
        for (int[] v : vectors) {
            int x = p.x + v[0];
            if (x < 0 || x >= pixels.length) {
                continue;
            }
            int y = p.y + v[1];
            if (y < 0 || y >= pixels[x].length) {
                continue;
            }

            int value = (pixels[x][y] & 0x0f000000) >> 24;

            if ((pixels[x][y] & 0x00ffffff) == recolor) {
                count = (count > value) ? count : value;
            }
        }

        return count;

    }

}
