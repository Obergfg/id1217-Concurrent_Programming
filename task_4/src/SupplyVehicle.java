import java.util.Random;
import static java.lang.Thread.sleep;

public class SupplyVehicle implements Runnable{

    private final FuelTank fuelTank;
    private final int fuelRequest;
    private final String fuelType;
    private final Random delay;
    private final FuelStation fuelStation;

    SupplyVehicle(int tankCapacity, String type, FuelStation fs){
        fuelTank = new FuelTank(tankCapacity);
        fuelRequest = tankCapacity/10;
        fuelType = type;
        delay = new Random();
        fuelStation = fs;
    }

    @Override
    public void run() {
        while (true){

                if(fuelStation.inNeedOfFuel(getFuelType())){
                    delivering();
                    refuelState();
                    leaving();
                    fuelTank.fillFuel();
                }

            try {
                sleep(2000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    void delivering() {

        boolean hasDelivered = false;
        try {
            System.out.println("A supply vehicle is on its way to deliver " + fuelType);
            sleep(delay.nextInt(1000));

            while (!hasDelivered){

                if(fuelStation.dockRequest()){
                    hasDocked();
                    if(fuelStation.supplyVehicleDock(this)){
                        hasDelivered= true;
                    }
                    fuelStation.leaveDock();
                }else
                    isQueueing(hasDelivered);
            }

            fuelStation.fuelSupplied(fuelType);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private void refuelState(){

        boolean hasRefueled = false;
        fueling();

        while (!hasRefueled) {

            if (fuelStation.dockRequest()) {
                if (fuelStation.requestNitrogen(getFuelRequest())) hasRefueled = true;
                fuelStation.leaveDock();
                if (!hasRefueled) isQueueing(true);
            } else
                isQueueing(true);
        }

    }

    void isQueueing(boolean hasDelivered){
        try {
            sleep(delay.nextInt(2000));
            if(hasDelivered)System.out.println("A supply vehicle has delivered " + fuelType + " and is queueing for a refuel before leaving the station");
            else System.out.println("A supply vehicle delivering " + fuelType + " is at the station and is queueing for a dock space");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void hasDocked(){
        try {
            sleep(2000);
            System.out.println("A supply vehicle delivering " + fuelType + " has docked to the fuel station and is setting up for unloading");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void leaving(){
        try {
            sleep(1000);
            System.out.println("A supply vehicle who delivered " + fuelType + " has refueled and is leaving the fuel station");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void fueling(){
        try {
            System.out.println("A supply vehicle has unloaded its cargo but need to refuel before leaving the station.");
            sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public String getFuelType() {
        return fuelType;
    }

    public FuelTank getFuelTank() {
        return fuelTank;
    }

    public int getFuelRequest() {
        return fuelRequest;
    }
}
