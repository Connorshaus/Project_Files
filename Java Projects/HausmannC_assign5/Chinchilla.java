public class Chinchilla extends Enemy {
    
    @Override
    public String toString() {
        return "Enemy name: CHINCHILLA Attack: BITE Health: " + this.health;
    }

    @Override
    public int fight() {
        // BITE ATTACK
        return damage.nextInt(50);
    }
    
}
