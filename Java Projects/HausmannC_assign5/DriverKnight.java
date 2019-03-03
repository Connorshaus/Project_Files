import java.util.ArrayList;
import java.util.Random;
 
import javax.swing.*;
 
 
public class DriverKnight
{
    public static void main(String[] args)
    {
        Knight k = new Knight();
        Random r1 = new Random();
        Random r2 = new Random();
        Random r3 = new Random();
        Random Who = new Random();
        int turn;
        boolean cont = true;
        
        while (cont == true)
        {
       
        JOptionPane.showMessageDialog(null, "Welcome to KnightFight!");
        
        k.setName(JOptionPane.showInputDialog("Enter a Name for your Knight!"));
        System.out.println("Knight Name: " + k.getName());
       
        k.setWep(Integer.parseInt(JOptionPane.showInputDialog("Now you must choose your weapon! (Select one of the following numbers) \n\n"
                + "1) Armadyl Godsword\n"
                + "2) Dragon Dagger\n"
                + "3) Abyssal Whip\n"
                + "4) Kick!")));
        System.out.println("Your choice: " + k.getWep());
       
        // Ask for the number of enemies
        int numberOfEnemies = (Integer.parseInt(JOptionPane.showInputDialog("How many enemies do you think you can take?")));
        // Create an Array of Enemies
        ArrayList<Enemy> enemies = new ArrayList<Enemy>();
        for(int i = 0; i < numberOfEnemies; i++) {
            Enemy e = Enemy.getRandomEnemy();
            e.setHealth(r2.nextInt(100)+1);
            enemies.add(e);
        }
       
        k.setHitbox(r2.nextInt(100)+1);
        System.out.println("You have " + k.getHitbox() + " health to start!");
        // Print all the opponent's health
        System.out.print("Your opponents have: ");
        for(int i = 0; i < enemies.size(); i++) 
            System.out.print(enemies.get(i).getHealth() + " ");
        System.out.println("health to start!");
        
        // Get all the message together, then print it out:
        StringBuilder message = new StringBuilder(k.toString() + " VS \n");
        for(int i = 0; i < enemies.size(); i++) {
            message.append(enemies.get(i).toString());
            message.append("\n");
        }
        JOptionPane.showMessageDialog(null, message.toString());
        
        JOptionPane.showMessageDialog(null, "Press OK to Fight!");
        
        turn = Who.nextInt(2);
        
        		
        while (k.Hitbox > 0 && enemies.size() > 0)
        {
        	if(turn % 2 == 0)
        	{
                    for(int i = 0; i < enemies.size(); i++) {
                        int damage = k.fight();
                        enemies.get(i).setHealth(enemies.get(i).getHealth() - damage);
                        JOptionPane.showMessageDialog(null, "Your knight dealt " + damage + " damage to the opponent!\n "
        				+ "Now your opponent is: " + enemies.get(i).toString());
                        // If the enemy is dead display a notice and remove it from the array
                        if(enemies.get(i).isDead()) {
                            JOptionPane.showMessageDialog(null, "This enemy is dead!");
                            enemies.remove(i);
                        }
                    }	
        	}
        	if(turn % 2 == 1)
        	{
                    int enemyNumber = r3.nextInt(enemies.size());
                    int damage = enemies.get(enemyNumber).fight();
        	    k.Hitbox = k.Hitbox - damage;
        	    JOptionPane.showMessageDialog(null, enemies.get(enemyNumber).toString() + " dealt " 
                        + damage + " damage to you!\n "
        		+ "You now have " + k.Hitbox + " health left!");
        	    if (k.Hitbox <=0) {	
        		JOptionPane.showMessageDialog(null, "You have died!\n We dont't have time for losers!");
        		break;
        	    }
        	}
        	turn++;
        }
        
        String again = JOptionPane.showInputDialog("Would you like to play again? \n\n"
                + "Yes\n"
                + "No");
        if(again.equals("Yes")){
            cont = true;
        } else {
            cont = false;
        }
        
        }
    }
}