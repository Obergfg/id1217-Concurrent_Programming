import java.util.Random;
import static java.lang.Thread.sleep;

public class SupplyVehicle implements Runnable{

    private final FuelTank fuelTank;
    private final int fuelRequest;
    private final String fuelType;
    private final Random flyingTime;
    private final FuelStation fuelStation;

    SupplyVehicle(int tankCapacity, String type, FuelStation fs){
        fuelTank = new FuelTank(tankCapacity);
        fuelRequest = tankCapacity/5;
        fuelType = type;
        flyingTime = new Random();
        fuelStation = fs;
    }

    @Override
    public void run() {

        while (true){

                if(fuelStation.inNeedOfFuel(getFuelType())){
                    delivering();
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
            sleep(flyingTime.nextInt(1000));

            while (!hasDelivered){

                if(fuelStation.dockRequest()){
                    hasDocked();
                    if(fuelStation.supplyVehicleDock(this)){
                        hasDelivered= true;
                    }
                    fuelStation.leaveDock();
                }else
                    isQueueing();
            }

            if(getFuelType().equals(" quantum fluid"))
                refuelState();

        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private void refuelState(){

        boolean hasRefueled = false;

        while (!hasRefueled) {

            if (fuelStation.dockRequest()) {
                if (fuelStation.requestNitrogen(getFuelRequest())) hasRefueled = true;
                fuelStation.leaveDock();
                if (!hasRefueled) isQueueing();
            } else
                isQueueing();
        }

    }

    void isQueueing(){
        try {
            sleep(1000);
            System.out.println("A supply vehicle delivering " + fuelType + " is at the station and is queueing for a dock space");
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
            System.out.println("A supply vehicle has unloaded its cargo of " + fuelType + " and is now leaving the fuel station");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void fueling(){
        try {
            System.out.println("A supply vehicle has unloaded its cargo but but need to wait for refueling before leaving the station.");
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
