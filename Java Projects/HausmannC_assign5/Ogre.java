
public class Ogre extends Enemy {
    
    @Override
    public String toString() {
        return "Enemy name: OGRE Attack: BRUTE FORCE Health: " + this.health;
    }

    @Override
    public int fight() {
        // BRUTE FORCE ATTACK
        return damage.nextInt(40);
    }
    
}
