package knn;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Enumeration;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.AbstractButton;
import javax.swing.ButtonGroup;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

public class Agregar_menu extends javax.swing.JFrame {

//Variables globales
    String ruta_fuente;

    public Agregar_menu() {
        initComponents();
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        grupo = new javax.swing.ButtonGroup();
        jPanel2 = new javax.swing.JPanel();
        jRadioButton_Xp = new javax.swing.JRadioButton();
        jRadioButton_Gp = new javax.swing.JRadioButton();
        jRadioButton_Mh = new javax.swing.JRadioButton();
        jRadioButton_Sc = new javax.swing.JRadioButton();
        jLabel1 = new javax.swing.JLabel();
        jLabel2 = new javax.swing.JLabel();
        jTextField_archivo = new javax.swing.JTextField();
        Boton_examinar = new javax.swing.JButton();
        jButton2 = new javax.swing.JButton();
        jPanel1 = new javax.swing.JPanel();
        jScrollPane1 = new javax.swing.JScrollPane();
        salida = new javax.swing.JTextArea();
        jScrollPane2 = new javax.swing.JScrollPane();
        muestra_contenido = new javax.swing.JTextArea();
        jLabel3 = new javax.swing.JLabel();
        jLabel5 = new javax.swing.JLabel();
        nombre_menú = new javax.swing.JTextField();
        jLabel4 = new javax.swing.JLabel();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Añadir menú");
        setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        setName("Añadir menú"); // NOI18N
        setResizable(false);

        grupo.add(jRadioButton_Xp);
        jRadioButton_Xp.setText("Xeon Phi");

        grupo.add(jRadioButton_Gp);
        jRadioButton_Gp.setText("GPU");
        jRadioButton_Gp.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jRadioButton_GpActionPerformed(evt);
            }
        });

        grupo.add(jRadioButton_Mh);
        jRadioButton_Mh.setText("Multihilos");

        grupo.add(jRadioButton_Sc);
        jRadioButton_Sc.setText("Secuencial");

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jRadioButton_Sc)
                    .addComponent(jRadioButton_Xp)
                    .addComponent(jRadioButton_Mh)
                    .addComponent(jRadioButton_Gp))
                .addContainerGap())
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jRadioButton_Sc)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jRadioButton_Mh)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jRadioButton_Xp)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jRadioButton_Gp)
                .addContainerGap())
        );

        jLabel1.setText("Tipo de menú");

        jLabel2.setText("Archivo fuente");

        jTextField_archivo.setEditable(false);
        jTextField_archivo.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jTextField_archivoActionPerformed(evt);
            }
        });

        Boton_examinar.setText("Examinar");
        Boton_examinar.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                Boton_examinarActionPerformed(evt);
            }
        });

        jButton2.setText("Compilar & añadir menú");
        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton2ActionPerformed(evt);
            }
        });

        salida.setEditable(false);
        salida.setColumns(20);
        salida.setRows(5);
        jScrollPane1.setViewportView(salida);

        muestra_contenido.setEditable(false);
        muestra_contenido.setColumns(20);
        muestra_contenido.setRows(5);
        jScrollPane2.setViewportView(muestra_contenido);

        jLabel3.setText("Archivo fuente");

        jLabel5.setText("Salida");

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createSequentialGroup()
                .addGap(40, 40, 40)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabel3)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 350, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 42, Short.MAX_VALUE)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 350, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel5))
                .addContainerGap(42, Short.MAX_VALUE))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGap(0, 0, Short.MAX_VALUE)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel3)
                    .addComponent(jLabel5))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 148, Short.MAX_VALUE)
                    .addComponent(jScrollPane2))
                .addContainerGap())
        );

        nombre_menú.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                nombre_menúActionPerformed(evt);
            }
        });

        jLabel4.setText("Nombre menú");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(jLabel1)
                        .addGap(52, 52, 52)
                        .addComponent(jLabel2))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(36, 36, 36)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(jTextField_archivo, javax.swing.GroupLayout.PREFERRED_SIZE, 188, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(Boton_examinar))))
                .addGap(40, 40, 40)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jLabel4)
                        .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(nombre_menú, javax.swing.GroupLayout.PREFERRED_SIZE, 176, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jButton2)
                        .addGap(42, 42, 42))))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel1)
                    .addComponent(jLabel4)
                    .addComponent(jLabel2, javax.swing.GroupLayout.PREFERRED_SIZE, 15, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(12, 12, 12)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jButton2, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 56, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(Boton_examinar)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(nombre_menú, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jTextField_archivo, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))))))
                .addGap(18, 18, 18)
                .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void jRadioButton_GpActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jRadioButton_GpActionPerformed
    }//GEN-LAST:event_jRadioButton_GpActionPerformed

    private void jTextField_archivoActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jTextField_archivoActionPerformed
    }//GEN-LAST:event_jTextField_archivoActionPerformed

    private void Boton_examinarActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_Boton_examinarActionPerformed
        JFileChooser explorador = new JFileChooser();
        explorador.setDialogTitle("Seleccionar archivo fuente...");
        int seleccion = explorador.showOpenDialog(this);
        File archivo = explorador.getSelectedFile();
        if (seleccion == 0) {
            jTextField_archivo.setText(archivo.getName());

            char var = 92;
            StringBuilder Ruta = new StringBuilder();
            String caracter = Character.toString(var);
            ruta_fuente = archivo.getPath();
            try {
                muestraContenido(archivo.getPath());
            } catch (IOException ex) {
                Logger.getLogger(Agregar_menu.class.getName()).log(Level.SEVERE, null, ex);
            }
        } else {
        }
    }//GEN-LAST:event_Boton_examinarActionPerformed

    private void nombre_menúActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_nombre_menúActionPerformed
    }//GEN-LAST:event_nombre_menúActionPerformed

    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton2ActionPerformed
        String sSistemaOperativo = System.getProperty("os.name");
        if (sSistemaOperativo.equals("Linux")) {
            String seleccion = getSelectedButtonText(grupo);
            chequear(seleccion);
            if ("Secuencial".equals(seleccion)) {
                try {
                        String path;
                        path = "/usr/lib/knn/Knn/Secuenciales/Fuentes/Knn_secuencial.out" +" "+ ruta_fuente;
                        Process p = Runtime.getRuntime().exec(path);
                        BufferedReader in = new BufferedReader(
                                new InputStreamReader(p.getInputStream()));
                        String line;
                        String texto = "algo";
                        System.out.println("Corriendo");
                        while ((line = in.readLine()) != null) {
                            texto = salida.getText() + "\n";
                            salida.setText(texto + line);
                            salida.updateUI();
                        }
                        p.waitFor();
                        System.out.println("Termino");
                        System.out.println(p.exitValue());
                        if(p.exitValue()==0){
                            JOptionPane.showMessageDialog(null, "Ha finalizado con exito");
                        }
                    } catch (IOException | InterruptedException e) {
                        JOptionPane.showMessageDialog(null, "Se ha producido un error \n" + e.getMessage());
                    }
            }
            if ("Multihilos".equals(seleccion)) {
                System.out.println("2");
            }

            if ("Xeon Phi".equals(seleccion)) {
                System.out.println("3");

            }
            if ("GPU".equals(seleccion)) {
                System.out.println("4");
            }
        }
    }//GEN-LAST:event_jButton2ActionPerformed

    public String getSelectedButtonText(ButtonGroup buttonGroup) {
        for (Enumeration<AbstractButton> buttons = buttonGroup.getElements(); buttons.hasMoreElements();) {
            AbstractButton button = buttons.nextElement();

            if (button.isSelected()) {
                return button.getText();
            }
        }

        return null;
    }

    public static void main(String args[]) {
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException | InstantiationException | IllegalAccessException | javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(Agregar_menu.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }

        java.awt.EventQueue.invokeLater(() -> {
            new Agregar_menu().setVisible(true);
        });
    }

    void chequear(String seleccion) {
        Object msj = "Faltan los siquientes argumentos:";
        String nl = System.getProperty("line.separator");
        if ("".equals(muestra_contenido.getText()) || "".equals(jTextField_archivo.getText())
                || "".equals(nombre_menú.getText()) || seleccion==null) {
            if (seleccion == null) {
                msj = msj + nl + "- No ha seleccionado el tipo de menú";
            }
            if ("".equals(jTextField_archivo.getText()) || "".equals(muestra_contenido.getText())) {
                msj = msj + nl + "- No se ha seleccionado un archivo fuente ";
            }
            if ("".equals(nombre_menú.getText())) {
                msj = msj + nl + "- No ha escrito el nombre del menú";
            }
            JOptionPane.showMessageDialog(null,
                    msj,
                    "Mensaje de Error",
                    JOptionPane.ERROR_MESSAGE
            );
        } else {

        }
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton Boton_examinar;
    private javax.swing.ButtonGroup grupo;
    private javax.swing.JButton jButton2;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JRadioButton jRadioButton_Gp;
    private javax.swing.JRadioButton jRadioButton_Mh;
    private javax.swing.JRadioButton jRadioButton_Sc;
    private javax.swing.JRadioButton jRadioButton_Xp;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JTextField jTextField_archivo;
    private javax.swing.JTextArea muestra_contenido;
    private javax.swing.JTextField nombre_menú;
    private javax.swing.JTextArea salida;
    // End of variables declaration//GEN-END:variables

    void muestraContenido(String archivo) throws FileNotFoundException, IOException {
        String cadena;
        muestra_contenido.setText(" ");
        int linea = 0;
        FileReader f = new FileReader(archivo);
        try (BufferedReader b = new BufferedReader(f)) {
            while ((cadena = b.readLine()) != null && linea < 100) {
                String texto = muestra_contenido.getText();
                muestra_contenido.setText(texto + cadena + "\n");
                linea++;
            }
        }
    }
}
