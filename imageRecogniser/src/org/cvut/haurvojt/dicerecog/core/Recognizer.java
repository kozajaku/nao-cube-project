/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.cvut.haurvojt.dicerecog.core;

import java.awt.Image;
import java.awt.Point;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;
import java.awt.image.PixelGrabber;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import javax.imageio.ImageIO;
import org.cvut.haurvojt.dicerecog.helpers.FileOutput;

/**
 * This is the main Dice Recognition class. Most parst of the algoritm for
 * recognizing dice value can be found here, appart from the preprocessing part,
 * for which see {@link Preprocessing} class. This class is also responsible for
 * the flow of the algoritm, in its methods you will find the algoritm steps.
 *
 * @author haurvojt
 */
public class Recognizer {

    private static int GLOBAL_INDEX = 0;

    public static int recognize(File imageFile) {

        BufferedImage img;
        try {
            img = ImageIO.read(imageFile);
        } catch (IOException e) {
            System.err.println(e);
            return -1;
        }

        return processImage(img, 0, 0, img.getWidth(), img.getHeight());

    }

    /**
     * This method will convert the 1D array into 2D and if necessary apply all preprocessing. Then will call 
     * next algoritm step to all subpictures received from preprocessing and select result from that calling.
     * 
     * @param pixelsSingle 1D array of pixels
     * @param w width of the 2D array
     * @param h height of the 2D array
     * @return estimated dice value
     */
    public static int processImageArray(int[] pixelsSingle, int w, int h) {

        // convert to 2D array
        int[][] pixels = new int[h][w];

        for (int j = 0; j < h; j++) {
            pixels[j] = Arrays.copyOfRange(pixelsSingle, j * w, (j + 1) * w);
        }

//        // testing purposes ----------------------------------------------------
//        FileOutput.writeOutput(h, w, pixels, "unmodified.png");
//        // testing preprocessing
//        int[][] redCapture = new int[h][w];
//        for (int i = 0; i < h; i++) {
//            for (int j = 0; j < w; j++) {
//                redCapture[i][j] = Common.isPixelRed(pixels[i][j]) ? pixels[i][j] : 0xffffffff;
//            }
//        }
//        FileOutput.writeOutput(h, w, redCapture, "redCapture.png");
//        // ---------------------------------------------------------------------
        
        List<SubpictureBundle> subPictures = Preprocessing.findPossibleDice(pixels, h, w);
        int index = 0;
        for (SubpictureBundle bundle : subPictures) {

            Distance.setDiceDistance(bundle.topPixelIndex);
            int[][] sp = bundle.pixels;
            FileOutput.writeOutput(sp.length, sp[0].length, sp, "subpicture_" + (index++) + ".png");

            // currently unsused enlarging and smoothing of the picture
//            sp = Preprocessing.enlarge(sp, 2);
//            sp = Preprocessing.smoothen(sp);
//            sp = Preprocessing.smoothen(sp);
            
            int result = handlePixels(sp, sp[0].length, sp.length);
            if (result >= 1 && result <= 6) {
                return result;
            }
        }

        return -1;
    }

    /**
     * This method will retrieve pixels as an int array from the given image
     * file and then process it as usual in the algoritm chain. This will result
     * in algorithm reading the dice value in the best case.
     * <br /> <br />
     * Currently only used for testing purposes as the robot will send his
     * vision already as an array.
     *
     * @param img Image to be converted into integer array
     * @param x where to start retrieving integers from the Image, usually will
     * be 0
     * @param y where to start retrieving integers from the Image, usually will
     * be 0
     * @param w width of the image
     * @param h height of the image
     * @return estimated dice value
     */
    public static int processImage(Image img, int x, int y, int w, int h) {
        int[] pixelsSingle = new int[w * h];

        PixelGrabber pg = new PixelGrabber(img, x, y, w, h, pixelsSingle, 0, w);
        try {
            pg.grabPixels();
        } catch (InterruptedException e) {
            System.err.println("interrupted waiting for pixels!");
            return -1;
        }
        if ((pg.getStatus() & ImageObserver.ABORT) != 0) {
            System.err.println("image fetch aborted or errored");
            return -1;
        }
        return processImageArray(pixelsSingle, w, h);

    }

    /**
     * This method will take subpicture and find clusters in it. Because it was
     * written before the preprocessing algorithm, which ensures that you only
     * have valid data in the 2D field, it does some steps unnecessary, which
     * can be good place for improvement.
     * <br /> <br />
     * <b>TODO:</b> remove parts of algoritm that are already ensured by cutting
     * out the subPicture
     *
     * @param pixels integer 2D array of pixels of the subpicture (one
     * DiceCluster)
     * @param w width of pixels
     * @param h height of pixe
     * @return estimated dice value
     */
    public static int handlePixels(int[][] pixels, int w, int h) {

        Set<Point> innerPoints = new HashSet<>();

        int cubeWidth = 0, cubeHeight = 0;
        int topRed = 0, leftmostRed = w;
        boolean redRowFound = false;

        for (int j = 0; j < h; j++) {

            boolean redInPath = false;
            int startRed = 0, endRed = 0;
            for (int i = 0; i < w; i++) {

                if (Common.isPixelRed(pixels[j][i])) {
                    if (!redInPath) {
                        redInPath = true;

                        startRed = i;
                    }
                    if (!redRowFound) {
                        redRowFound = true;
                        topRed = j;
                    }

                    if (i < leftmostRed) {
                        leftmostRed = i;
                    }

                    endRed = i;
                    cubeHeight = j - topRed;

                } else {

                    if (Common.DICE_DOT_RECOGNIZER.isDotColor(pixels[j][i])) {
//                    if (redInPath && Common.DICE_DOT_RECOGNIZER.isDotColor(pixels[j][i])) {
                        // if the pixel is dark then it should be a DOT on the dice
                        if (recolorPixel(pixels, j, i, h, w)) {
                            pixels[j][i] = 0xff66ff33;
                            innerPoints.add(new Point(j, i));
                        }

                    } else {
                        // temporary color to be recolored to red 
                        pixels[j][i] = 0x12345678;
                    }
                }

            }

            if (cubeWidth < endRed - startRed + 1) {
                cubeWidth = endRed - startRed + 1;
            }
        }

        for (int j = 0; j < h; j++) {

            for (int i = 0; i < w; i++) {
                if (pixels[j][i] == 0x12345678) {
                    // recolor temporary color to red
                    pixels[j][i] = 0xffff0000;
                }
            }
        }

        Ellipse ellipse = new Ellipse(cubeHeight, cubeWidth, leftmostRed, topRed);

        System.out.println("Clustering");
        List<List<Point>> clusters = new ArrayList<>();
        while (!innerPoints.isEmpty()) {
            Point first = innerPoints.iterator().next();
            innerPoints.remove(first);
            List<Point> cluster = new ArrayList<>();
            cluster.add(first);
            Common.recursiveCluster(innerPoints, first, cluster);

            clusters.add(cluster);
        }

        System.out.println("Advanced processing");
        List<Point> dicePoints = ClusterProcessing.advancedProcessCluster(clusters, pixels, ellipse);




        // testing purposes drawing ellipse into the picture - no real need for the computation
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                double value = (i - ellipse.x0) * (i - ellipse.x0) / (ellipse.a * ellipse.a) 
                        + (j - ellipse.y0) * (j - ellipse.y0) / (ellipse.b * ellipse.b);
                if (value > 0.995 && value < 1.025) {
                    pixels[j][i] = 0xff0000ff;
                }

            }
        }
        System.out.println("Elipse drawn");
        // end of drawing ellipse for testing purposes 

        FileOutput.writeOutput(h, w, pixels, "output" + (GLOBAL_INDEX++) + ".png");

        
        // remove all the points that do not lie inside od the ellipse
        Iterator<Point> iterator = dicePoints.iterator();
        while(iterator.hasNext()){
            Point p = iterator.next();
            if(!ellipse.isPointInside(p)){
                iterator.remove();
            }
        }
        
        dicePoints = mergePointsTooClose(dicePoints);
        
        int result = dicePoints.size();
//        
//        for (Point point : dicePoints) {
//            if (ellipse.isPointInside(point)) {
//                result++;
//            }
//        }
//        
        

        System.out.println("RESULT is " + result);

        return result;
    }
    
    public static List<Point> mergePointsTooClose(List<Point> points){
        double averagedDistance = 0.0;
        double counter = 0;
        for(int i = 0; i < points.size(); i++){
            for (int j = i + 1; j < points.size(); j++) {
                averagedDistance += points.get(i).distance(points.get(j));
                counter++;
            }
        }
        
        averagedDistance /= counter;
        
        Set<Point> tempPoints = new HashSet<>(points);
        List<Point> refinedPoints = new ArrayList<>();
//        while(!tempPoints.isEmpty()){
//            
//        }
        for(int i = 0; i < points.size(); i++){
            if(!tempPoints.contains(points.get(i))){
                continue;
            }
            boolean merged = false;
            for (int j = i + 1; j < points.size(); j++) {
                if(points.get(i).distance(points.get(j)) < averagedDistance * 0.25){
                    Point pi = points.get(i), pj = points.get(j);
                    tempPoints.remove(pi);
                    tempPoints.remove(pj);
                    refinedPoints.add(new Point((pi.x + pj.x) / 2, (pi.y + pj.y) / 2));
                    System.err.println("MERGED");
                    merged = true;
                    break;
                    
                } 
                
            }
            if(!merged){
                tempPoints.remove(points.get(i));
                refinedPoints.add(points.get(i));
            }
        }
        
        return refinedPoints;
    }

    public static boolean recolorPixel(int[][] pixels, int x, int y, int h, int w) {

        return isRedInPath(pixels, x, y, h, w, 1, 0)
                && isRedInPath(pixels, x, y, h, w, 0, 1)
                && isRedInPath(pixels, x, y, h, w, 0, -1) // left not needed
                && isRedInPath(pixels, x, y, h, w, -1, 0);

    }

    private static boolean isRedInPath(int[][] pixels, int x, int y, int h, int w, int vectX, int vectY) {
        return (redInPathDist(pixels, x, y, h, w, vectX, vectY) < Integer.MAX_VALUE);

    }

    private static int redInPathDist(int[][] pixels, int x, int y, int h, int w, int vectX, int vectY) {

        for (int j = x + vectX, i = y + vectY; i >= 0 && i < w && j >= 0 && j < h;) {
            if (Common.isPixelRed(pixels[j][i])) {

                return Math.abs(j - x) + Math.abs(i - y);
            }
            j = j + vectX;
            i = i + vectY;
        }
        return Integer.MAX_VALUE;
    }

}
