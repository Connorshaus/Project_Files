import java.util.Random;
 
import javax.swing.*;
 
 
public class DriverKnight
{
    public static void main(String[] args)
    {
        Knight k = new Knight();
        Knight k2 = new Knight();
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
       
        String user = JOptionPane.showInputDialog("Would you like to auto-generate your opponent? \n\n"
                + "Yes\n"
                + "No");
       
        if(user.equals("yes") || user.equals("Yes"))
        {
            k2.setName("Random Knight");
           
            k2.setWep(r1.nextInt(4) + 1);
        }
        if(user.equals("no") || user.equals("No"))
        {
            k2.setName(JOptionPane.showInputDialog("Enter a Name for your Opponent!"));
            System.out.println("Opponent Name: " + k2.getName());
            k2.setWep(Integer.parseInt(JOptionPane.showInputDialog("Now you must choose your opponent's weapon! (Select one of the following numbers) \n\n"
                    + "1) Long Sword\n"
                    + "2) Dagger\n"
                    + "3) War Hammer\n"
                    + "4) Bare Fists")));
            System.out.println("Your choice: " + k2.getWep());
        }
       
        k.setHitbox(r2.nextInt(100)+1);
        System.out.println("You have " + k.getHitbox() + " health to start!");
        k2.setHitbox(r3.nextInt(100)+1);
        System.out.println("Your opponent has " + k2.getHitbox() + " health to start!");
        
        JOptionPane.showMessageDialog(null, k.toString() + "\n" + k2.toString());
        
        JOptionPane.showMessageDialog(null, "Press OK to Fight!");
        
        turn = Who.nextInt(2);
        
        		
        while (k.Hitbox > 0 && k2.Hitbox > 0)
        {
        	if(turn % 2 == 0)
        	{
        		k2.Hitbox = k2.Hitbox - k.fight();
        		JOptionPane.showMessageDialog(null, "Your knight dealt " + k.fight() + " damage to the opponent!\n "
        				+ "He now has " + k2.Hitbox + " health left!");
        		if (k2.Hitbox <=0)
        		{	
        			JOptionPane.showMessageDialog(null, "The opponent has died!\n You are Victorious!");
        			break;
        		}
        			
        	}
        	if(turn % 2 == 1)
        	{
        		k.Hitbox = k.Hitbox - k2.fight();
        		JOptionPane.showMessageDialog(null, "Your opponent dealt " + k2.fight() + " damage to you!\n "
        				+ "You now have " + k.Hitbox + " health left!");
        		if (k.Hitbox <=0)
        		{	
        			JOptionPane.showMessageDialog(null, "You have died!\n We dont't have time for losers!");
        			break;
        		}
        	}
        	turn++;
        }
        
        String user2 = JOptionPane.showInputDialog("Would you like to play again? \n\n"
                + "Yes\n"
                + "No");
        if(user2.equals("yes") || user.equals("Yes"))
        {
            cont = true;
        }
        if(user2.equals("no") || user.equals("No"))
        {
            cont = false;
        }
        
        
        }
    
    
    
    
    }
}