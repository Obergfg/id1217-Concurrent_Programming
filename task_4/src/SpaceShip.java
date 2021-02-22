import java.util.Random;
import static java.lang.Thread.sleep;

public class SpaceShip implements Runnable {

    private final int shipID;
    private final int nitrogenRefuelLevel;
    private final int quantumFluidRefuelLevel;
    private final FuelTank nitrogen;
    private final FuelTank quantumFluid;
    private final FuelStation fuelStation;
    private final Random flyingTime;
    private String state;
    private int dockingTurnsLeft;

    SpaceShip(int id, int nitrogenCapacity, int quantumFluidCapacity, int dockingTurns ,FuelStation fs) {
        shipID = id;
        nitrogenRefuelLevel = nitrogenCapacity / 2;
        quantumFluidRefuelLevel = quantumFluidCapacity / 2;
        dockingTurnsLeft = dockingTurns;
        nitrogen = new FuelTank(nitrogenCapacity);
        quantumFluid = new FuelTank(quantumFluidCapacity);
        fuelStation = fs;
        flyingTime = new Random();
    }

    @Override
    public void run() {

        setFlyingState();

        while (0 < dockingTurnsLeft) {

            flying(false);

            if (nitrogen.getFuelVolume() < nitrogenRefuelLevel && quantumFluid.getFuelVolume() < quantumFluidRefuelLevel){
                setBothFuelsState();
                fuelStation.spaceShipDock(this);
                dockingTurnsLeft--;
            } else if (nitrogen.getFuelVolume() < nitrogenRefuelLevel) {
                setNitrogenState();
                fuelStation.spaceShipDock(this);
                dockingTurnsLeft--;
            } else if(quantumFluid.getFuelVolume() < quantumFluidRefuelLevel){
                setQuantumState();
                fuelStation.spaceShipDock(this);
                dockingTurnsLeft--;
            }else
                decreaseFuel();
        }

        System.out.println("Space ship " + getShipID() + " has finished its tasks!");
    }

    void fillNitrogen() {
        nitrogen.fillFuel();
        try {
            sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void fillQuantumFluid(){
        quantumFluid.fillFuel();
        try {
            sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private void decreaseFuel() {
        nitrogen.setFuelVolume(nitrogen.getFuelVolume() - 1);
        quantumFluid.setFuelVolume(quantumFluid.getFuelVolume() - flyingTime.nextInt(4));
    }


    void flying(boolean atStation) {

        String message;

        if(atStation)
            message = "Ship " + getShipID() + " is at the station and is flying out of the dock since it could not be serviced at the moment";
        else
            message = "Ship " + shipID + " is flying and has " + nitrogen.getFuelVolume() + " nitrogen and " + quantumFluid.getFuelVolume() + " quantum fluid units left.";

        try {
            System.out.println(message);
            sleep(flyingTime.nextInt(4000));
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void hasDocked(){
        System.out.println("Ship " + getShipID() + " has docked to the fuel station and is requesting " + getRequestState());
        try {
            sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void isQueueing(){
        System.out.println("Ship " + getShipID() + " is at the station and is queueing for a dock space");
        try {
            sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void setFlyingState() {
        this.state = "flying";
    }

    void setNitrogenState() {
        this.state = "nitrogen";
    }

    void setQuantumState(){
        this.state = "quantum fluid";
    }

    void setBothFuelsState(){
        this.state = "both fuels";
    }

    public String getRequestState() {
        return state;
    }

    public int getShipID() {
        return shipID;
    }

    public FuelTank getQuantumFluid() {
        return quantumFluid;
    }

    public FuelTank getNitrogen() {
        return nitrogen;
    }
}
