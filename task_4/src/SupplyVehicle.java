import java.util.Random;

import static java.lang.Thread.sleep;

public class SupplyVehicle implements Runnable{

    private final FuelTank fuelTank;
    private final int fuelRequest;
    private boolean supplyRequest;
    private final String fuelType;
    private final Random flyingTime;
    private final FuelStation fuelStation;

    SupplyVehicle(int tankCapacity, String type, FuelStation fs){
        fuelTank = new FuelTank(tankCapacity);
        fuelRequest = tankCapacity/5;
        supplyRequest = false;
        fuelType = type;
        flyingTime = new Random();
        fuelStation = fs;
    }

    @Override
    public void run() {

        while (true){

                if(supplyRequest){
                    delivering();
                    fuelStation.supplyVehicleDock(this);
                    leaving();
                    fuelTank.fillFuel();
                }

        }



    }

    void requestFuelSupply(){
        supplyRequest = true;
    }

    void delivering() {
        try {
            System.out.println("A supply vehicle is on its way to deliver " + fuelType);
            sleep(flyingTime.nextInt(4000));
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void isQueueing(){
        try {
            System.out.println("A supply vehicle delivering " + fuelType + "is at the station and is queueing for a dock space");
            sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void hasDocked(){
        try {
            System.out.println("A supply vehicle delivering " + fuelType + " has docked to the fuel station and is setting up for unloading");
            sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void leaving(){
        try {
            System.out.println("A supply vehicle has unloaded its cargo of " + fuelType + " and is now leaving the fuel station");
            setSupplyRequest(false);
            sleep(10000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }

    public String getFuelType() {
        return fuelType;
    }

    public void setSupplyRequest(boolean supplyRequest) {
        this.supplyRequest = supplyRequest;
    }
}
