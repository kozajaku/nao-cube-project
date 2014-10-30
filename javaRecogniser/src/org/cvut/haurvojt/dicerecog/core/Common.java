/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package org.cvut.haurvojt.dicerecog.core;

import org.cvut.haurvojt.dicerecog.colors.DiceColorRecognizer;
import org.cvut.haurvojt.dicerecog.colors.RedDiceColorRecognizer;
import java.awt.Point;
import java.util.List;
import java.util.Set;
import org.cvut.haurvojt.dicerecog.colors.DarkDiceDotRecognizer;
import org.cvut.haurvojt.dicerecog.colors.DiceDotRecognizer;

/**
 *
 * @author haurvojt
 */
public class Common {
    
    
    
    
    public static final DiceColorRecognizer DICE_COLOR_RECOGNIZER = new RedDiceColorRecognizer();
    public static final DiceDotRecognizer DICE_DOT_RECOGNIZER = new DarkDiceDotRecognizer();
    
    public static boolean isPixelRed(int pixel) {

        return DICE_COLOR_RECOGNIZER.isDiceColor(pixel);
    }
    
    public static int[][] getSurrounding(){
        int[][] vectors = new int[8][2];
        int counter = 0;
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if (i == 0 && j == 0) {
                    continue;
                }
                vectors[counter++] = new int[]{i, j};
            }
        }
        return vectors;
    }
    
    public static void recursiveCluster(Set<Point> points, Point center, List<Point> clusteredPoints) {

        Point[] vectors = {new Point(1, 0), new Point(-1, 0), new Point(0, 1), new Point(0, -1)};
        Point[] siders = new Point[vectors.length+1];
        siders[0] = null;
        int index = 0;
        for (Point vec : vectors) {
            Point sidePoint = new Point(center.x + vec.x, center.y + vec.y);
            if (points.contains(sidePoint)) {
                points.remove(sidePoint);
                clusteredPoints.add(sidePoint);
                siders[index++] = sidePoint; 
                siders[index] = null;
            }
        }
        
        for(int i = 0; i < siders.length; i++){
            if(siders[i] == null){
                break;
            }
            recursiveCluster(points, siders[i], clusteredPoints);
        }
    }
    
}
