import java.awt.Container;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;

import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;


 
public class TicTacToe_GUI extends JFrame
{
    private Container pane;
    private String currentPlayer;
    private JButton [][] board;
    private boolean hasWinner;
    private JMenuBar menuBar;
    private JMenu menu;
    private JMenuItem quit;
    private JMenuItem restart;
    private JMenuItem about;
   
    public TicTacToe_GUI()
    {
        super();
        pane = getContentPane();
        pane.setLayout(new GridLayout(3, 3));
        setTitle("FSU Tic Tac Toe");
        setSize (500,500);
        setResizable(true);
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        setVisible (true);
        currentPlayer = "Seminoles";
        board = new JButton [3][3];
        hasWinner = false;
        initializeBoard();
        initializeMenuBar();
        
        

        // create the status bar panel and shove it down the bottom of the frame
        //JPanel statusPanel = new JPanel();                                                         This is my attempt at adding a footer.
        //statusPanel.setBorder(new BevelBorder(BevelBorder.LOWERED));                               Unfortunately it did not work
        //pane.add(statusPanel, BorderLayout.SOUTH);
        //statusPanel.setPreferredSize(new Dimension(pane.getWidth(), 24));
        //statusPanel.setLayout(new BoxLayout(statusPanel, BoxLayout.X_AXIS));
        //JLabel statusLabel = new JLabel("status");
        //statusLabel.setHorizontalAlignment(SwingConstants.CENTER);
        //statusPanel.add(statusLabel);

        pane.setVisible(true);
        
    }
   
    private void initializeMenuBar()
    {
        menuBar = new JMenuBar();
        menu = new JMenu("File");
        restart = new JMenuItem("New Game");
        restart.addActionListener(new ActionListener()
        {
                @Override
                public void actionPerformed (ActionEvent e) {
                    resetBoard();
                }  
        });
       
        quit = new JMenuItem("Quit");
        quit.addActionListener(new ActionListener()
        {
                @Override
                public void actionPerformed (ActionEvent e) {
                    System.exit(0);
                }  
        });
        about = new JMenuItem("About");
        about.addActionListener(new ActionListener()
        {
                @Override
                public void actionPerformed (ActionEvent e) {
                	 JOptionPane.showMessageDialog(null, "Created by Connor Hausmann.\n"                //This added in replacement
                	 		+ "Logic based on videos by \"NoobCoder\" on YouTube.");                    //for the footer :(
                }  
        });
        
        menu.add(restart);
        menu.add(quit);
        menu.add(about);
        menuBar.add(menu);
        setJMenuBar(menuBar);
    }
   
    private void resetBoard()
    {
        currentPlayer = "Seminoles";
        hasWinner = false;
        for (int i = 0; i < 3; i ++)
        {  
            for (int j = 0; j < 3; j++)
                    board[i][j].setText("");
        }
    }
   
    private void initializeBoard ()
    {
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                JButton btn = new JButton ("");
                btn.setFont(new Font(Font.SANS_SERIF,Font.BOLD,25));
                board[i][j] = btn;
                btn.addActionListener(new ActionListener()
                        {
                            @Override
                            public void actionPerformed(ActionEvent e)
                            {
                                if(((JButton)e.getSource()).getText().equals("")
                                && hasWinner == false)
                                    {
                                        btn.setText(currentPlayer);
                                        hasWinner();
                                        togglePlayer();
                                    }
                            }
                        });
                pane.add(btn);
            }
        }
           
    }
   
    private void togglePlayer()
    {
        if(currentPlayer.equals("Seminoles"))
            currentPlayer = "Gators";
        else
            currentPlayer = "Seminoles";
    }
   
    private void hasWinner()
    {
        if(board[0][0].getText().equals(currentPlayer) && board[1][0].getText().equals(currentPlayer) && board [2][0].getText().equals(currentPlayer))
        {
            JOptionPane.showMessageDialog(null, "Player " + currentPlayer + " has won!");
            hasWinner = true;
        }
        else if(board[0][1].getText().equals(currentPlayer) && board[1][1].getText().equals(currentPlayer) && board [2][1].getText().equals(currentPlayer))
        {
            JOptionPane.showMessageDialog(null, "Player " + currentPlayer + " has won!");
            hasWinner = true;
        }
        else if(board[0][2].getText().equals(currentPlayer) && board[1][2].getText().equals(currentPlayer) && board [2][2].getText().equals(currentPlayer))
        {
            JOptionPane.showMessageDialog(null, "Player " + currentPlayer + " has won!");
            hasWinner = true;
        }
        else if(board[0][0].getText().equals(currentPlayer) && board[1][1].getText().equals(currentPlayer) && board [2][2].getText().equals(currentPlayer))
        {
            JOptionPane.showMessageDialog(null, "Player " + currentPlayer + " has won!");
            hasWinner = true;
        }
        else if(board[2][0].getText().equals(currentPlayer) && board[1][1].getText().equals(currentPlayer) && board [0][2].getText().equals(currentPlayer))
        {
            JOptionPane.showMessageDialog(null, "Player " + currentPlayer + " has won!");
            hasWinner = true;
        }
        else if(board[0][0].getText().equals(currentPlayer) && board[0][1].getText().equals(currentPlayer) && board [0][2].getText().equals(currentPlayer))
        {
            JOptionPane.showMessageDialog(null, "Player " + currentPlayer + " has won!");
            hasWinner = true;
        }
        else if(board[1][0].getText().equals(currentPlayer) && board[1][1].getText().equals(currentPlayer) && board [1][2].getText().equals(currentPlayer))
        {
            JOptionPane.showMessageDialog(null, "Player " + currentPlayer + " has won!");
            hasWinner = true;
        }
        else if(board[2][0].getText().equals(currentPlayer) && board[2][1].getText().equals(currentPlayer) && board [2][2].getText().equals(currentPlayer))
        {
            JOptionPane.showMessageDialog(null, "Player " + currentPlayer + " has won!");
            hasWinner = true;
        }
       
    }
}