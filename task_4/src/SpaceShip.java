import java.util.Random;

import static java.lang.Thread.sleep;

public class SpaceShip implements Runnable {

    private final int shipID;
    private final int nitrogenRefuelLevel;
    private final int quantumFluidRefuelLevel;
    private final FuelTank nitrogenTank;
    private final FuelTank quantumFluidTank;
    private final FuelStation fuelStation;
    private final Random flyingTime;
    private int stationsVisitsLeft;

    SpaceShip(int id, int nitrogenCapacity, int quantumFluidCapacity, int dockingTurns, FuelStation fs) {
        shipID = id;
        nitrogenRefuelLevel = nitrogenCapacity / 2;
        quantumFluidRefuelLevel = quantumFluidCapacity / 2;
        stationsVisitsLeft = dockingTurns;
        nitrogenTank = new FuelTank(nitrogenCapacity);
        quantumFluidTank = new FuelTank(quantumFluidCapacity);
        fuelStation = fs;
        flyingTime = new Random();
    }

    @Override
    public void run() {
        while (0 < stationsVisitsLeft) {

            if (nitrogenTank.getFuelVolume() < nitrogenRefuelLevel && quantumFluidTank.getFuelVolume() < quantumFluidRefuelLevel) {
                bothRefuelFuelState();
                stationsVisitsLeft--;
            } else if (nitrogenTank.getFuelVolume() < nitrogenRefuelLevel) {
                refuelState("nitrogen", getNitrogenTank().getFuelVolume(), true);
                stationsVisitsLeft--;
            } else if (quantumFluidTank.getFuelVolume() < quantumFluidRefuelLevel) {
                refuelState("quantum fluid", getQuantumFluidTank().getFuelVolume(), true);
                stationsVisitsLeft--;
            } else
                decreaseFuel();

            flying(false);
        }

        System.out.println("Space ship " + getShipID() + " has finished its tasks!");
        fuelStation.finishedShip();
    }

    private void bothRefuelFuelState() {

        System.out.println("Ship " + getShipID() + " has only " + getNitrogenTank().getFuelVolume() + " " +
                "units of nitrogen left and " + getQuantumFluidTank().getFuelVolume() + " units of " +
                "quantum fluid left and is heading for a refuel");

        refuelState("nitrogen", getNitrogenTank().getFuelVolume(), false);
        refuelState("quantum fluid", getQuantumFluidTank().getFuelVolume(), false);

        System.out.println("Ship " + getShipID() + " has refueled both nitrogen and quantum fluid and is heading " +
                "out of the fuel station");
    }

    private void refuelState(String fuelType, int fuelVolume, boolean showMessage) {
        boolean hasRefueled = false;

        if (showMessage)
            System.out.println("Ship " + getShipID() + " has only " + fuelVolume + " units of " + fuelType + " left " +
                    "and is heading for a refuel");

        while (!hasRefueled) {

            if (fuelStation.dockRequest()) {
                hasDocked(fuelType);

                if(fuelType.equals("nitrogen")){
                    if (fuelStation.requestNitrogen(getNitrogenEmptyVolume())) {
                        fillNitrogen();
                        hasRefueled = true;
                    }
                }else {
                    if (fuelStation.requestQuantumFluid(getQuantumFluidEmptyVolume())) {
                        fillQuantumFluid();
                        hasRefueled = true;
                    }
                }

                fuelStation.leaveDock();

                if (!hasRefueled)
                    flying(true);
            } else
                isQueueing();
        }

        if (showMessage)
            System.out.println("Ship " + getShipID() + " has refueled " + fuelType + " and is heading out of the fuel station");
    }


    void fillNitrogen() {
        nitrogenTank.fillFuel();
        try {
            sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void fillQuantumFluid() {
        quantumFluidTank.fillFuel();
        try {
            sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private void decreaseFuel() {
        nitrogenTank.setFuelVolume(nitrogenTank.getFuelVolume() - 1);
        quantumFluidTank.setFuelVolume(quantumFluidTank.getFuelVolume() - flyingTime.nextInt(3));
    }

    void flying(boolean atStation) {

        if (atStation)
            System.out.println("Ship " + getShipID() + " is at the station and is flying out of the dock since it could not be serviced at the moment");

        try {
            sleep(flyingTime.nextInt(5000));
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void hasDocked(String fuel) {
        System.out.println("Ship " + getShipID() + " has docked to the fuel station and is requesting " + fuel);
        try {
            sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    void isQueueing() {
        System.out.println("Ship " + getShipID() + " is at the station and is queueing for a dock space");
        try {
            sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    int getNitrogenEmptyVolume() {
        return this.nitrogenTank.getCapacity() - this.nitrogenTank.getFuelVolume();
    }

    int getQuantumFluidEmptyVolume() {
        return this.quantumFluidTank.getCapacity() - this.quantumFluidTank.getFuelVolume();
    }

    public int getShipID() {
        return shipID;
    }

    public FuelTank getQuantumFluidTank() {
        return quantumFluidTank;
    }

    public FuelTank getNitrogenTank() {
        return nitrogenTank;
    }
}
