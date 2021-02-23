public class Main {

    public static void main(String[] args){
        System.out.println("Program started!");

        int nitrogenCapacity = 80;
        int quantumFluidCapacity = 80;
        int ships = 5;

        FuelStation fuelStation = new FuelStation(2, nitrogenCapacity, quantumFluidCapacity, ships);
        SupplyVehicle nitrogenSupplyVehicle = new SupplyVehicle(nitrogenCapacity/2, "nitrogen", fuelStation);
        SupplyVehicle quantumFluidSupplyVehicle = new SupplyVehicle(quantumFluidCapacity/2, "quantum fluid", fuelStation);

        Thread n = new Thread(nitrogenSupplyVehicle);
        Thread q = new Thread(quantumFluidSupplyVehicle);
        n.start();
        q.start();

        SpaceShip[] spaceShips = new SpaceShip[ships];

        for(int i = 1; i <= spaceShips.length; i++){
           Thread t = new Thread(spaceShips[i - 1] = new SpaceShip(i,20, 20, 4, fuelStation));
           t.start();
        }
    }
}
