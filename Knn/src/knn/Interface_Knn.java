package knn;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFileChooser;

public class Interface_Knn extends javax.swing.JFrame {

    String Ruta_DB;
    String Ruta_Queries;
    String Num_threads;
    boolean flag_Secuencial = false;
    boolean flag_Multihilos = false;
    boolean flag_Xenon_Phi = false;
    boolean flag_GPU = false;

    public Interface_Knn() {
        this.Ruta_Queries = null;
        this.Ruta_DB = null;
        this.Num_threads = null;
        flag_Secuencial = false;
        flag_Multihilos = false;
        flag_Xenon_Phi = false;
        flag_GPU = false;
        initComponents();
        flag_GPU = false;
        flag_Multihilos = false;
        flag_Xenon_Phi = false;
        flag_Secuencial = false;
        jLabel_Hilos.setVisible(false);
        jTextField_Hilos.setVisible(false);
        jButton_Examinar_BD.setVisible(false);
        jButton_Examinar_Queries.setVisible(false);
        jButton_Exportar_PDF.setVisible(false);
        jButton_Hilos.setVisible(false);
        jLabel_Archivo_BD.setVisible(false);
        jLabel_Archivo_Queries.setVisible(false);
        jLabel_Hilos.setVisible(false);
        jTextArea_Vista_Queries.setVisible(false);
        jTextField_Archivo_BD.setVisible(false);
        jTextField_Archivo_Queries.setVisible(false);
        setTitle("Knn: Finder nearest neighbor");
        setSize(1000, 641);
        setResizable(false);
        setLocationRelativeTo(null);
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jButton2 = new javax.swing.JButton();
        jPanel_Archivos = new javax.swing.JPanel();
        jLabel_Archivo_BD = new javax.swing.JLabel();
        jLabel_Archivo_Queries = new javax.swing.JLabel();
        jLabel_Hilos = new javax.swing.JLabel();
        jTextField_Archivo_BD = new javax.swing.JTextField();
        jTextField_Archivo_Queries = new javax.swing.JTextField();
        jTextField_Hilos = new javax.swing.JTextField();
        jButton_Examinar_BD = new javax.swing.JButton();
        jButton_Hilos = new javax.swing.JButton();
        jButton_Examinar_Queries = new javax.swing.JButton();
        jSeparator1 = new javax.swing.JSeparator();
        jLabel_Archivo_Queries3 = new javax.swing.JLabel();
        jTextField_Archivo_Queries3 = new javax.swing.JTextField();
        jTextField_Archivo_Queries4 = new javax.swing.JTextField();
        jLabel_TOPK = new javax.swing.JLabel();
        jRadioButton_Int = new javax.swing.JRadioButton();
        Float = new javax.swing.JRadioButton();
        jRadioButton_Double = new javax.swing.JRadioButton();
        jPanel1 = new javax.swing.JPanel();
        jScrollPane2 = new javax.swing.JScrollPane();
        jTextArea_Vista_Queries = new javax.swing.JTextArea();
        jScrollPane3 = new javax.swing.JScrollPane();
        jTextArea_Resultados = new javax.swing.JTextArea();
        jScrollPane4 = new javax.swing.JScrollPane();
        jTextArea_Vista_DB = new javax.swing.JTextArea();
        jLabel1 = new javax.swing.JLabel();
        jLabel_VistaBD = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        jLabel_Vista_Queries = new javax.swing.JLabel();
        jLabel_Vista_Queries1 = new javax.swing.JLabel();
        jPanel2 = new javax.swing.JPanel();
        jButton_Exportar_PDF = new javax.swing.JButton();
        Menu_Principal = new javax.swing.JMenuBar();
        jMenu_File = new javax.swing.JMenu();
        JMenu_Nuevo = new javax.swing.JMenu();
        jMenuItem_Secuencial = new javax.swing.JMenuItem();
        jMenuItem_Multihilos = new javax.swing.JMenuItem();
        jMenuItem_Xenon_Phi = new javax.swing.JMenuItem();
        jMenuItem_GPU = new javax.swing.JMenuItem();
        jMenuItem_Salir = new javax.swing.JMenuItem();
        jMenu_Edit = new javax.swing.JMenu();

        jButton2.setText("jButton1");

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setPreferredSize(new java.awt.Dimension(1000, 600));
        setResizable(false);

        jLabel_Archivo_BD.setText("Archivo BD");

        jLabel_Archivo_Queries.setText("Archivo Queries");

        jLabel_Hilos.setText("Hilos");

        jButton_Examinar_BD.setText("Examinar");
        jButton_Examinar_BD.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton_Examinar_BDActionPerformed(evt);
            }
        });

        jButton_Hilos.setText("Aceptar");
        jButton_Hilos.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton_HilosActionPerformed(evt);
            }
        });

        jButton_Examinar_Queries.setText("Examinar");
        jButton_Examinar_Queries.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton_Examinar_QueriesActionPerformed(evt);
            }
        });

        jLabel_Archivo_Queries3.setText("Dimensión Obj");

        jLabel_TOPK.setText("TOPK");

        jRadioButton_Int.setText("Int");

        Float.setText("Float");
        Float.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                FloatActionPerformed(evt);
            }
        });

        jRadioButton_Double.setText("Double");

        javax.swing.GroupLayout jPanel_ArchivosLayout = new javax.swing.GroupLayout(jPanel_Archivos);
        jPanel_Archivos.setLayout(jPanel_ArchivosLayout);
        jPanel_ArchivosLayout.setHorizontalGroup(
            jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel_Archivo_BD)
                            .addComponent(jTextField_Archivo_BD, javax.swing.GroupLayout.PREFERRED_SIZE, 150, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(jButton_Examinar_BD))
                        .addGap(35, 35, 35)
                        .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jButton_Examinar_Queries)
                            .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                                .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(jLabel_Archivo_Queries)
                                    .addComponent(jTextField_Archivo_Queries, javax.swing.GroupLayout.PREFERRED_SIZE, 150, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGap(35, 35, 35)
                                .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                                    .addComponent(jLabel_Archivo_Queries3, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                    .addComponent(jTextField_Archivo_Queries3))
                                .addGap(35, 35, 35)
                                .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(jLabel_TOPK)
                                    .addComponent(jTextField_Archivo_Queries4, javax.swing.GroupLayout.PREFERRED_SIZE, 100, javax.swing.GroupLayout.PREFERRED_SIZE))))
                        .addGap(35, 35, 35)
                        .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel_Hilos)
                            .addComponent(jTextField_Hilos, javax.swing.GroupLayout.PREFERRED_SIZE, 67, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(35, 35, 35))
                    .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                        .addComponent(jSeparator1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)))
                .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                        .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jRadioButton_Int)
                            .addComponent(Float))
                        .addGap(100, 100, 100)
                        .addComponent(jButton_Hilos)
                        .addGap(27, 27, 27))
                    .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                        .addComponent(jRadioButton_Double)
                        .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
        );
        jPanel_ArchivosLayout.setVerticalGroup(
            jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(jRadioButton_Int)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(Float)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jRadioButton_Double)
                        .addGap(42, 42, 42))
                    .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                        .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                                    .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jLabel_Archivo_BD)
                                        .addComponent(jLabel_Archivo_Queries))
                                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                    .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jTextField_Archivo_BD, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addComponent(jTextField_Archivo_Queries, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                                    .addComponent(jLabel_Archivo_Queries3)
                                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                    .addComponent(jTextField_Archivo_Queries3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                                    .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jLabel_TOPK)
                                        .addComponent(jLabel_Hilos))
                                    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                    .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                        .addComponent(jTextField_Archivo_Queries4, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addComponent(jTextField_Hilos, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))))
                            .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                                .addGap(25, 25, 25)
                                .addComponent(jButton_Hilos)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jButton_Examinar_BD)
                            .addComponent(jButton_Examinar_Queries))
                        .addGap(32, 32, 32)))
                .addComponent(jSeparator1, javax.swing.GroupLayout.PREFERRED_SIZE, 0, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );

        jTextArea_Vista_Queries.setColumns(20);
        jTextArea_Vista_Queries.setRows(5);
        jScrollPane2.setViewportView(jTextArea_Vista_Queries);

        jTextArea_Resultados.setColumns(20);
        jTextArea_Resultados.setRows(5);
        jScrollPane3.setViewportView(jTextArea_Resultados);

        jTextArea_Vista_DB.setColumns(20);
        jTextArea_Vista_DB.setRows(5);
        jScrollPane4.setViewportView(jTextArea_Vista_DB);

        jLabel1.setText("Vista Previa: ");

        jLabel_VistaBD.setText("Archivo BD");

        jLabel3.setText("Vista Previa: ");

        jLabel_Vista_Queries.setText("Archivo Queries");

        jLabel_Vista_Queries1.setText("Resultado: ");

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane4, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addComponent(jLabel1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabel_VistaBD)))
                .addGap(31, 31, 31)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addComponent(jLabel3)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabel_Vista_Queries)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane3, javax.swing.GroupLayout.PREFERRED_SIZE, 390, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel_Vista_Queries1))
                .addContainerGap())
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createSequentialGroup()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGap(0, 0, Short.MAX_VALUE)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(jLabel1)
                            .addComponent(jLabel_VistaBD)
                            .addComponent(jLabel3)
                            .addComponent(jLabel_Vista_Queries))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED))
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(jLabel_Vista_Queries1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 10, Short.MAX_VALUE)))
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jScrollPane3, javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jScrollPane4, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, 350, Short.MAX_VALUE)))
        );

        jButton_Exportar_PDF.setText("Exportar PDF");

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(jButton_Exportar_PDF)
                .addGap(43, 43, 43))
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(jButton_Exportar_PDF)
                .addGap(39, 39, 39))
        );

        jMenu_File.setText("Archivo");

        JMenu_Nuevo.setText("Nuevo");

        jMenuItem_Secuencial.setText("Secuencial");
        jMenuItem_Secuencial.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem_SecuencialActionPerformed(evt);
            }
        });
        JMenu_Nuevo.add(jMenuItem_Secuencial);

        jMenuItem_Multihilos.setText("Multihilos");
        jMenuItem_Multihilos.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem_MultihilosActionPerformed(evt);
            }
        });
        JMenu_Nuevo.add(jMenuItem_Multihilos);

        jMenuItem_Xenon_Phi.setText("Xenon Phi");
        JMenu_Nuevo.add(jMenuItem_Xenon_Phi);

        jMenuItem_GPU.setText("GPU");
        JMenu_Nuevo.add(jMenuItem_GPU);

        jMenu_File.add(JMenu_Nuevo);

        jMenuItem_Salir.setText("Salir");
        jMenu_File.add(jMenuItem_Salir);

        Menu_Principal.add(jMenu_File);

        jMenu_Edit.setText("Edit");
        Menu_Principal.add(jMenu_Edit);

        setJMenuBar(Menu_Principal);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(jPanel_Archivos, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addComponent(jPanel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel_Archivos, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, 75, Short.MAX_VALUE)
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void jButton_Examinar_BDActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton_Examinar_BDActionPerformed
        JFileChooser explorador = new JFileChooser();
        explorador.setDialogTitle("Seleccionar DB...");
        int seleccion = explorador.showOpenDialog(this);
        File archivo = explorador.getSelectedFile();
        if (seleccion == 0) {
            jTextField_Archivo_BD.setText(archivo.getName());
            Ruta_DB = archivo.getPath();
            try {
                muestraContenidoDB(Ruta_DB);
            } catch (IOException ex) {
                Logger.getLogger(Interface_Knn.class.getName()).log(Level.SEVERE, null, ex);
            }
        } else {

        }
    }//GEN-LAST:event_jButton_Examinar_BDActionPerformed

    private void jButton_Examinar_QueriesActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton_Examinar_QueriesActionPerformed
        JFileChooser explorador = new JFileChooser();
        explorador.setDialogTitle("Seleccionar Queries...");
        int seleccion = explorador.showOpenDialog(this);
        File archivo = explorador.getSelectedFile();
        if (seleccion == 0) {
            jTextField_Archivo_Queries.setText(archivo.getName());
            Ruta_Queries = archivo.getPath();
            try {
                muestraContenidoQueries(Ruta_Queries);
            } catch (IOException ex) {
                Logger.getLogger(Interface_Knn.class.getName()).log(Level.SEVERE, null, ex);
            }
        } else {

        }
    }//GEN-LAST:event_jButton_Examinar_QueriesActionPerformed

    private void jButton_HilosActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton_HilosActionPerformed
        String sSistemaOperativo = System.getProperty("os.name");
        System.out.println(sSistemaOperativo);
        if (sSistemaOperativo.equals("Linux")) {
            if (flag_Secuencial == true) {
                try {
                    jTextArea_Resultados.setText("Consulta Secuencial...");
                    String path;
                    String Ejecutable_Secuencial = "/home/cristofher/Tesis/Secuencial.out";
                    path = Ejecutable_Secuencial + " " + Ruta_DB + " 95325 " + Ruta_Queries + " 23831";
                    Process p = Runtime.getRuntime().exec(path);
                    BufferedReader in = new BufferedReader(
                            new InputStreamReader(p.getInputStream()));
                    String line;
                    String texto;
                    while ((line = in.readLine()) != null) {
                        texto = jTextArea_Resultados.getText() + "\n";
                        System.out.println(line);
                        jTextArea_Resultados.setText(texto + line);
                        jTextArea_Resultados.updateUI();
                    }
                } catch (IOException e) {

                }
            }
        }
        if (flag_Multihilos == true) {
            try {
                jTextArea_Resultados.setText("Consulta con multihilos...");
                String path;
                Num_threads = jTextField_Hilos.getText();
                String Ejecutable_Multihilos = "/home/cristofher/Tesis/Multihilos.out";
                path = Ejecutable_Multihilos + " " + Ruta_DB + " 95325 " + Ruta_Queries + " 23831 " + Num_threads;
                Process p = Runtime.getRuntime().exec(path);

                BufferedReader in = new BufferedReader(
                        new InputStreamReader(p.getInputStream()));
                String line;
                String texto;
                while ((line = in.readLine()) != null) {
                    texto = jTextArea_Resultados.getText() + "\n";
                    System.out.println(line);
                    jTextArea_Resultados.append(path);
                    jTextArea_Resultados.setText(texto + line);
                }
            } catch (IOException e) {

            }
        }
    }//GEN-LAST:event_jButton_HilosActionPerformed

    private void jMenuItem_SecuencialActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem_SecuencialActionPerformed
        flag_GPU = false;
        flag_Multihilos = false;
        flag_Xenon_Phi = false;
        flag_Secuencial = true;
        jLabel_Hilos.setVisible(false);
        jTextField_Hilos.setVisible(false);
        jButton_Examinar_BD.setVisible(true);
        jButton_Examinar_Queries.setVisible(true);
        jButton_Exportar_PDF.setVisible(true);
        jButton_Hilos.setVisible(true);
        jLabel_Archivo_BD.setVisible(true);
        jLabel_Archivo_Queries.setVisible(true);
        jLabel_Hilos.setVisible(false);
        jTextArea_Vista_Queries.setVisible(true);
        jTextField_Archivo_BD.setVisible(true);
        jTextField_Archivo_Queries.setVisible(true);
        jTextField_Hilos.setVisible(false);
    }//GEN-LAST:event_jMenuItem_SecuencialActionPerformed

    private void jMenuItem_MultihilosActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem_MultihilosActionPerformed
        flag_GPU = false;
        flag_Multihilos = true;
        flag_Xenon_Phi = false;
        flag_Secuencial = false;
        jLabel_Hilos.setVisible(true);
        jTextField_Archivo_BD.setVisible(true);
        jTextField_Archivo_Queries.setVisible(true);
        jTextField_Hilos.setVisible(true);
        jButton_Examinar_BD.setVisible(true);
        jButton_Examinar_Queries.setVisible(true);
        jButton_Exportar_PDF.setVisible(true);
        jButton_Hilos.setVisible(true);
        jLabel_Archivo_BD.setVisible(true);
        jLabel_Archivo_Queries.setVisible(true);
        jLabel_Hilos.setVisible(true);
        jTextArea_Vista_Queries.setVisible(true);
    }//GEN-LAST:event_jMenuItem_MultihilosActionPerformed

    private void FloatActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_FloatActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_FloatActionPerformed

    void muestraContenidoDB(String archivo) throws FileNotFoundException, IOException {
        String cadena;
        jTextArea_Vista_DB.setText(" ");
        int linea = 0;
        FileReader f = new FileReader(archivo);
        try (BufferedReader b = new BufferedReader(f)) {
            while ((cadena = b.readLine()) != null && linea < 100) {
                String texto = jTextArea_Vista_DB.getText();
                jTextArea_Vista_DB.setText(texto + cadena + "\n");
                linea++;
            }
        }
    }

    void muestraContenidoQueries(String archivo) throws FileNotFoundException, IOException {
        String cadena;
        int linea = 0;
        FileReader f = new FileReader(archivo);
        try (BufferedReader b = new BufferedReader(f)) {
            while ((cadena = b.readLine()) != null && linea < 100) {
                String texto = jTextArea_Vista_Queries.getText();
                jTextArea_Vista_Queries.setText(texto + cadena + "\n");
                linea++;
            }
        }
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
            java.util.logging.Logger.getLogger(Interface_Knn.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }

        java.awt.EventQueue.invokeLater(() -> {
            new Interface_Knn().setVisible(true);
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JRadioButton Float;
    private javax.swing.JMenu JMenu_Nuevo;
    private javax.swing.JMenuBar Menu_Principal;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton_Examinar_BD;
    private javax.swing.JButton jButton_Examinar_Queries;
    private javax.swing.JButton jButton_Exportar_PDF;
    private javax.swing.JButton jButton_Hilos;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel_Archivo_BD;
    private javax.swing.JLabel jLabel_Archivo_Queries;
    private javax.swing.JLabel jLabel_Archivo_Queries3;
    private javax.swing.JLabel jLabel_Hilos;
    private javax.swing.JLabel jLabel_TOPK;
    private javax.swing.JLabel jLabel_VistaBD;
    private javax.swing.JLabel jLabel_Vista_Queries;
    private javax.swing.JLabel jLabel_Vista_Queries1;
    private javax.swing.JMenuItem jMenuItem_GPU;
    private javax.swing.JMenuItem jMenuItem_Multihilos;
    private javax.swing.JMenuItem jMenuItem_Salir;
    private javax.swing.JMenuItem jMenuItem_Secuencial;
    private javax.swing.JMenuItem jMenuItem_Xenon_Phi;
    private javax.swing.JMenu jMenu_Edit;
    private javax.swing.JMenu jMenu_File;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel_Archivos;
    private javax.swing.JRadioButton jRadioButton_Double;
    private javax.swing.JRadioButton jRadioButton_Int;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JScrollPane jScrollPane3;
    private javax.swing.JScrollPane jScrollPane4;
    private javax.swing.JSeparator jSeparator1;
    private javax.swing.JTextArea jTextArea_Resultados;
    private javax.swing.JTextArea jTextArea_Vista_DB;
    private javax.swing.JTextArea jTextArea_Vista_Queries;
    private javax.swing.JTextField jTextField_Archivo_BD;
    private javax.swing.JTextField jTextField_Archivo_Queries;
    private javax.swing.JTextField jTextField_Archivo_Queries3;
    private javax.swing.JTextField jTextField_Archivo_Queries4;
    private javax.swing.JTextField jTextField_Hilos;
    // End of variables declaration//GEN-END:variables
}
