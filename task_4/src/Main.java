public class Main {

    public static void main(String[] args){
        System.out.println("Program started!");
        FuelStation fuelStation = new FuelStation(2, 20, 20);
        SpaceShip[] spaceShips = new SpaceShip[3];

        for(int i = 1; i <= spaceShips.length; i++){
           Thread t = new Thread(spaceShips[i - 1] = new SpaceShip(i,10, 10, 3, fuelStation));
           t.start();
        }
    }
}
