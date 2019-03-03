/*
Hausmann, Connor
COP-3252
Assignment 6
Date: 4/15/2018
*/

// ArrayAccess.java
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class ArrayAccess extends JFrame 
{
   private JTextField inputField;
   private JTextField retrieveField1;
   private JTextField retrieveField2;
   private JTextField outputField;
   private JPanel inputArea; 
   private JPanel retrieveArea;
   private JPanel outputArea;

   private int num;
   private int index = 0;
   private int array[] = new int[ 10 ];
   private String result;
   
   // set up GUI
   public ArrayAccess()
   {
      super( "Accessing Array values" );
      setLayout( new FlowLayout() );
      
      // set up input Panel
      inputArea = new JPanel();
      inputArea.add( new JLabel( "Enter array elements here" ) );
      inputField = new JTextField( 10 );
      inputArea.add( inputField );
      inputField.addActionListener( 
         new ActionListener()
         {
            public void actionPerformed( ActionEvent e )
            {
            	//
            	try {
            		array[index] = Integer.parseInt(inputField.getText());  //converts the array into integer
            		index++;												//increases the array element by 1
            	}catch( NumberFormatException ex) { 						//catches the number format exception if the format is incorrect
            		JOptionPane.showMessageDialog(null, "Please enter only integer values" + ex.getMessage(), "Invalid Input" , JOptionPane.ERROR_MESSAGE);
            	}
            	catch( ArrayIndexOutOfBoundsException ex){					//Prints out error message if the array contains too many elements
            		JOptionPane.showMessageDialog(null, "Array may contain only 10 elements",  "Array Full" , JOptionPane.ERROR_MESSAGE);
      			}
               inputField.setText( "" );
            } // end method actionPerformed
         } // end anonymous inner class
      ); // end call to addActionListener
      
      // set up retrieve Panel
      retrieveArea = new JPanel( new GridLayout( 2, 2 ) );
      retrieveArea.add( new JLabel( "Enter number to retrieve" ) );
      retrieveField1 = new JTextField( 10 );
      retrieveArea.add( retrieveField1 );
      retrieveField1.addActionListener( 
         new ActionListener() 
         {
            public void actionPerformed( ActionEvent event ) 
            {
            	//
            	try {
            		num = Integer.parseInt(retrieveField1.getText());
            		boolean isHere = false;									//sets the boolean 'isHere' to false
            		result = "";
            		System.out.println(result.length());
					for (int i = 0; i < array.length; i++) 					//runs the user through the array
					{
						if(num == array[i]) 
						{
							isHere = true;									//if the number matches the array's index value, 'isHere' is set to true
							result += i + " ";								// the I is added to the result
						}
            		}
					
					if(!isHere) 
					{
						throw new NumberNotFoundException();				//exception is thrown if isHere is not here (!isHere)
					}														
					outputField.setText(num + " was in the following fields of the array: " + result);
            		
            	}catch(NumberFormatException ex) {							//performed if the user enters, non0integer values
            		JOptionPane.showMessageDialog(null, "Please enter only integer values" + ex.getMessage(), "Invalid Input" , JOptionPane.ERROR_MESSAGE);
                	
            	} catch (NumberNotFoundException ex) {						//performed when the number is not found, thows the number not found exception
            		JOptionPane.showMessageDialog(null, ex.getMessage(), "Not Found" , JOptionPane.ERROR_MESSAGE);
				}
               retrieveField1.setText( "" );
            } // end method actionPerformed
         } // end anonymous inner class
      ); // end call to addActionListener
            
      retrieveArea.add( new JLabel( "Enter index to retrieve" ) );
      retrieveField2 = new JTextField( 10 );
      retrieveArea.add( retrieveField2 );
      retrieveField2.addActionListener(
         new ActionListener() 
         {
            public void actionPerformed( ActionEvent event )
            {
            	try {
            		//
            		num = Integer.parseInt(retrieveField2.getText());
            		outputField.setText(String.valueOf(array[num]));
            	}
            	catch(NumberFormatException ex) {
            		JOptionPane.showMessageDialog(null, "Please enter only integer values " + ex.getMessage(), "Invalid Error" , JOptionPane.ERROR_MESSAGE);
                	
            	}catch (ArrayIndexOutOfBoundsException ex) {				//thrown when the user wishes to access an index that is not being used
            		JOptionPane.showMessageDialog(null, "Index Not Found: " + ex.getMessage(), "Index Out of Bounds" , JOptionPane.ERROR_MESSAGE);
          			
            	}
               retrieveField2.setText( "" );
            } // end anonymous inner class
         } // end new ActionListener
      ); // end call to addActionListener
      
      // set up output Panel
      outputArea = new JPanel();
      outputArea.add( new JLabel( "Result" ) );
      outputField = new JTextField( 30 );
      outputField.setEditable( false );
      outputArea.add( outputField );

      add( inputArea );
      add( retrieveArea );
      add( outputArea );
   }  // end constructor
} // end class ArrayAccess