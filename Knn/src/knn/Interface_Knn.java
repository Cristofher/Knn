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
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Interface_Knn extends javax.swing.JFrame {

    //Ruta de bases de datos
    String Ruta_DB;
    String Ruta_Queries;

    //Archivos con todos los codigos fuentes
    String[] rutas_Secuenciales;
    String[] rutas_Multihilos;
    String[] rutas_Xeon_phi;
    String[] rutas_GPU;

    //Ruta de ejecutable
    String Ejecutable_Secuencial;
    String Ejecutable_Multihilos;
    String Ejecutable_Xeon_Phi;
    String Ejecutable_GPU;

    //Archivos con nombres del menu 
    String[] nombres_Secuenciales;
    String[] nombres_Multihilos;
    String[] nombres_Xeon_phi;
    String[] nombres_GPU;

    JMenuItem[] menus_Secuencial;

    int num_secuenciales = 0;

    String Num_threads;
    String TOPK;
    String DIM;

    int tamanho_DB;
    int tamanho_Queries;

    //Banderas 
    boolean flag_Secuencial = false;
    boolean flag_Multihilos = false;
    boolean flag_Xenon_Phi = false;
    boolean flag_GPU = false;

    public Interface_Knn() throws IOException {
        initComponents();
        Ruta_Queries = null;
        Ruta_DB = null;
        Num_threads = null;
        flag_Secuencial = false;
        flag_Multihilos = false;
        flag_Xenon_Phi = false;
        flag_GPU = false;
        jLabel_Hilos.setVisible(false);
        jTextField_Hilos.setVisible(false);
        jButton_Examinar_BD.setVisible(false);
        jButton_Examinar_Queries.setVisible(false);
        jButton_Exportar_PDF.setVisible(false);
        jButton_Procesar_Consultas.setVisible(false);
        jLabel_Archivo_BD.setVisible(false);
        jLabel_Archivo_Queries.setVisible(false);
        jLabel_Hilos.setVisible(false);
        jLabel_Archivo_Queries3.setVisible(false);
        jTextField_Dim.setVisible(false);
        jLabel_TOPK.setVisible(false);
        jTextField_TOPK.setVisible(false);
        jTextArea_Vista_DB.setVisible(false);
        jLabel1.setVisible(false);
        jLabel3.setVisible(false);
        jLabel_Vista_Queries.setVisible(false);
        jTextArea_Resultados.setVisible(false);
        jLabel_Vista_Queries1.setVisible(false);
        jLabel2.setVisible(false);
        jButton_Exportar_PDF1.setVisible(false);
        jTextArea_Resultados.setVisible(false);
        jTextArea_Vista_Queries.setVisible(false);
        jTextField_Archivo_BD.setVisible(false);
        jTextField_Archivo_Queries.setVisible(false);

        //INICIO Lectura de archivos secuenciales
        String cadena;
        FileReader CfSecuenciales = new FileReader("Knn/Secuenciales/Fuentes/fuentes.dat");
        BufferedReader b = new BufferedReader(CfSecuenciales);

        while ((cadena = b.readLine()) != null) {
            if (!"".equals(cadena)) {
                num_secuenciales++;
            }
        }

        rutas_Secuenciales = new String[num_secuenciales];
        nombres_Secuenciales = new String[num_secuenciales];
        menus_Secuencial = new JMenuItem[num_secuenciales];
        num_secuenciales = 0;
        FileReader CfSecuenciales2 = new FileReader("Knn/Secuenciales/Fuentes/fuentes.dat");
        BufferedReader b2 = new BufferedReader(CfSecuenciales2);
        while ((cadena = b2.readLine()) != null) {
            if (!"".equals(cadena)) {
                rutas_Secuenciales[num_secuenciales] = cadena;
                System.out.println(cadena);
            }
            num_secuenciales++;
        }
        b.close();

        FileReader MnSecuenciales = new FileReader("Knn/Secuenciales/Menus/nombres.dat");
        b = new BufferedReader(MnSecuenciales);
        for (int i = 0; i < num_secuenciales; i++) {
            nombres_Secuenciales[i] = b.readLine();
            System.out.println(nombres_Secuenciales[i]);
            menus_Secuencial[i] = new JMenuItem(nombres_Secuenciales[i]);
            menus_Secuencial[i].addActionListener(new MenuActionSecuencial());
            jMenu_Secuencial.add(menus_Secuencial[i]);
        }
        b.close();
        //FIN Lectura de archivos secuenciales

        //INICIO Lectura de archivos Multihilos
        FileReader CfMultihilos = new FileReader("Knn/Multihilos/Fuentes/fuentes.dat");
        b = new BufferedReader(CfMultihilos);
        int num_multihilos = 0;
        while ((cadena = b.readLine()) != null) {
            if (!"".equals(cadena)) {
                num_multihilos++;
            }
        }
        rutas_Multihilos = new String[num_multihilos];
        nombres_Multihilos = new String[num_multihilos];
        num_multihilos = 0;
        CfMultihilos = new FileReader("Knn/Multihilos/Fuentes/fuentes.dat");
        b = new BufferedReader(CfMultihilos);
        while ((cadena = b.readLine()) != null) {
            if (!"".equals(cadena)) {
                rutas_Multihilos[num_multihilos] = cadena;
            }
            num_multihilos++;
        }
        b.close();

        FileReader MnMultihilos = new FileReader("Knn/Multihilos/Menus/nombres.dat");
        b = new BufferedReader(MnMultihilos);
        for (int i = 0; i < num_multihilos; i++) {
            nombres_Multihilos[i] = b.readLine();
            JMenuItem menu = new JMenuItem(nombres_Multihilos[i]);
            jMenu_Multihilos.add(menu);
        }
        b.close();
        //FIN Lectura de archivos Multihilos

        //INICIO Lectura de archivos Xeon Phi
        System.out.println("aca todo bien");
        FileReader CfXeon_phi = new FileReader("Knn/Xeon_Phi/Fuentes/fuentes.dat");
        b = new BufferedReader(CfXeon_phi);
        int num_xeon_phi = 0;
        while ((cadena = b.readLine()) != null) {
            if (!"".equals(cadena)) {
                num_xeon_phi++;
            }
        }
        rutas_Xeon_phi = new String[num_xeon_phi];
        nombres_Xeon_phi = new String[num_xeon_phi];
        num_xeon_phi = 0;
        CfXeon_phi = new FileReader("Knn/Xeon_Phi/Fuentes/fuentes.dat");
        b = new BufferedReader(CfXeon_phi);
        while ((cadena = b.readLine()) != null) {
            if (!"".equals(cadena)) {
                System.out.println(cadena);
                rutas_Xeon_phi[num_xeon_phi] = cadena;
            }
            num_xeon_phi++;
        }
        b.close();
        FileReader MnXeon_phi = new FileReader("Knn/Xeon_Phi/Menus/nombres.dat");
        b = new BufferedReader(MnXeon_phi);
        for (int i = 0; i < num_xeon_phi; i++) {
            nombres_Xeon_phi[i] = b.readLine();
            System.out.println(nombres_Xeon_phi[i]);
            JMenuItem menu = new JMenuItem(nombres_Xeon_phi[i]);
            jMenu_Xeon_phi.add(menu);
        }
        b.close();
        //FIN Lectura de archivos XeonPhi

        //INICIO Lectura de archivos Nvidia GPU
        FileReader CfGpu = new FileReader("Knn/Gpu/Fuentes/fuentes.dat");
        b = new BufferedReader(CfGpu);
        int num_gpu = 0;
        while ((cadena = b.readLine()) != null) {
            if (!"".equals(cadena)) {
                num_gpu++;
            }
        }
        rutas_GPU = new String[num_gpu];
        nombres_GPU = new String[num_gpu];
        num_gpu = 0;
        CfGpu = new FileReader("Knn/Gpu/Fuentes/fuentes.dat");
        b = new BufferedReader(CfGpu);
        while ((cadena = b.readLine()) != null) {
            if (!"".equals(cadena)) {
                rutas_GPU[num_gpu] = cadena;
            }
            num_gpu++;
        }
        b.close();

        FileReader MnGpu = new FileReader("Knn/Gpu/Menus/nombres.dat");
        b = new BufferedReader(MnGpu);
        for (int i = 0; i < num_gpu; i++) {
            nombres_GPU[i] = b.readLine();
            JMenuItem menu = new JMenuItem(nombres_GPU[i]);
            jMenu_GPU.add(menu);
        }
        b.close();

        //FIN Lectura de archivos Nvidia GPU
        //String procesadores = rutas[0];
        //Nucleo.setText(procesadores);
        jTextField_Hilos.setText(Nucleo.getText());
        setTitle("Knn: Finder nearest neighbor");
        setResizable(false);
        setLocationRelativeTo(null);

    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jButton2 = new javax.swing.JButton();
        Tipo = new javax.swing.ButtonGroup();
        jPanel_Archivos = new javax.swing.JPanel();
        jLabel_Archivo_BD = new javax.swing.JLabel();
        jLabel_Archivo_Queries = new javax.swing.JLabel();
        jLabel_Hilos = new javax.swing.JLabel();
        jTextField_Archivo_BD = new javax.swing.JTextField();
        jTextField_Archivo_Queries = new javax.swing.JTextField();
        jTextField_Hilos = new javax.swing.JTextField();
        jButton_Examinar_BD = new javax.swing.JButton();
        jButton_Procesar_Consultas = new javax.swing.JButton();
        jButton_Examinar_Queries = new javax.swing.JButton();
        jSeparator1 = new javax.swing.JSeparator();
        jLabel_Archivo_Queries3 = new javax.swing.JLabel();
        jTextField_Dim = new javax.swing.JTextField();
        jTextField_TOPK = new javax.swing.JTextField();
        jLabel_TOPK = new javax.swing.JLabel();
        jPanel1 = new javax.swing.JPanel();
        jScrollPane2 = new javax.swing.JScrollPane();
        jTextArea_Vista_Queries = new javax.swing.JTextArea();
        jScrollPane3 = new javax.swing.JScrollPane();
        jTextArea_Resultados = new javax.swing.JTextArea();
        jScrollPane4 = new javax.swing.JScrollPane();
        jTextArea_Vista_DB = new javax.swing.JTextArea();
        jLabel1 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        jLabel_Vista_Queries = new javax.swing.JLabel();
        jLabel_Vista_Queries1 = new javax.swing.JLabel();
        jButton_Exportar_PDF = new javax.swing.JButton();
        jLabel2 = new javax.swing.JLabel();
        jButton_Exportar_PDF1 = new javax.swing.JButton();
        jPanel2 = new javax.swing.JPanel();
        Nucleo = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        jLabel5 = new javax.swing.JLabel();
        Menu_Principal = new javax.swing.JMenuBar();
        jMenu_File = new javax.swing.JMenu();
        JMenu_Nuevo = new javax.swing.JMenu();
        jMenu_Secuencial = new javax.swing.JMenu();
        jMenu_Multihilos = new javax.swing.JMenu();
        jMenu_Xeon_phi = new javax.swing.JMenu();
        jMenu_GPU = new javax.swing.JMenu();
        jMenuItem_Secuencial = new javax.swing.JMenuItem();
        jMenuItem_Multihilos = new javax.swing.JMenuItem();
        jMenuItem_Xenon_Phi = new javax.swing.JMenuItem();
        jMenuItem_GPU = new javax.swing.JMenuItem();
        jMenuItem_Salir = new javax.swing.JMenuItem();
        jMenu_Edit = new javax.swing.JMenu();

        jButton2.setText("jButton1");

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setResizable(false);

        jPanel_Archivos.setBorder(javax.swing.BorderFactory.createTitledBorder("Entrada de datos"));

        jLabel_Archivo_BD.setText("Archivo BD");

        jLabel_Archivo_Queries.setText("Archivo Queries");

        jLabel_Hilos.setText("Hilos");

        jTextField_Archivo_BD.setEditable(false);

        jTextField_Archivo_Queries.setEditable(false);

        jTextField_Hilos.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyTyped(java.awt.event.KeyEvent evt) {
                jTextField_HilosKeyTyped(evt);
            }
        });

        jButton_Examinar_BD.setText("Examinar");
        jButton_Examinar_BD.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton_Examinar_BDActionPerformed(evt);
            }
        });

        jButton_Procesar_Consultas.setFont(new java.awt.Font("Ubuntu", 1, 15)); // NOI18N
        jButton_Procesar_Consultas.setText("Procesar Consultas");
        jButton_Procesar_Consultas.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton_Procesar_ConsultasActionPerformed(evt);
            }
        });

        jButton_Examinar_Queries.setText("Examinar");
        jButton_Examinar_Queries.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton_Examinar_QueriesActionPerformed(evt);
            }
        });

        jLabel_Archivo_Queries3.setText("Dimensión Obj");

        jTextField_Dim.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyTyped(java.awt.event.KeyEvent evt) {
                jTextField_DimKeyTyped(evt);
            }
        });

        jTextField_TOPK.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyTyped(java.awt.event.KeyEvent evt) {
                jTextField_TOPKKeyTyped(evt);
            }
        });

        jLabel_TOPK.setText("TOPK");

        javax.swing.GroupLayout jPanel_ArchivosLayout = new javax.swing.GroupLayout(jPanel_Archivos);
        jPanel_Archivos.setLayout(jPanel_ArchivosLayout);
        jPanel_ArchivosLayout.setHorizontalGroup(
            jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                                .addComponent(jLabel_Archivo_BD)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(jButton_Examinar_BD))
                            .addComponent(jTextField_Archivo_BD, javax.swing.GroupLayout.PREFERRED_SIZE, 200, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(30, 30, 30)
                        .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addGroup(jPanel_ArchivosLayout.createSequentialGroup()
                                .addComponent(jLabel_Archivo_Queries)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(jButton_Examinar_Queries))
                            .addComponent(jTextField_Archivo_Queries, javax.swing.GroupLayout.PREFERRED_SIZE, 200, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(30, 30, 30)
                        .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel_Archivo_Queries3)
                            .addComponent(jTextField_Dim, javax.swing.GroupLayout.PREFERRED_SIZE, 80, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(30, 30, 30)
                        .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel_TOPK)
                            .addComponent(jTextField_TOPK, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(30, 30, 30)
                        .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel_Hilos)
                            .addComponent(jTextField_Hilos, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addGap(32, 32, 32))
                    .addComponent(jSeparator1))
                .addGap(30, 30, 30)
                .addComponent(jButton_Procesar_Consultas)
                .addContainerGap())
        );
        jPanel_ArchivosLayout.setVerticalGroup(
            jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel_ArchivosLayout.createSequentialGroup()
                .addGap(12, 12, 12)
                .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel_Archivo_Queries)
                    .addComponent(jLabel_Archivo_Queries3)
                    .addComponent(jLabel_TOPK)
                    .addComponent(jLabel_Hilos)
                    .addComponent(jButton_Examinar_Queries)
                    .addComponent(jButton_Examinar_BD)
                    .addComponent(jLabel_Archivo_BD))
                .addGroup(jPanel_ArchivosLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jTextField_Archivo_Queries, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jTextField_Dim, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jTextField_TOPK, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jTextField_Hilos, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jButton_Procesar_Consultas, javax.swing.GroupLayout.PREFERRED_SIZE, 60, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jTextField_Archivo_BD, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 11, Short.MAX_VALUE)
                .addComponent(jSeparator1, javax.swing.GroupLayout.PREFERRED_SIZE, 0, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(30, 30, 30))
        );

        jPanel1.setBorder(javax.swing.BorderFactory.createTitledBorder("Resultados (Se muestran las primeras 1000 lineas)"));

        jTextArea_Vista_Queries.setEditable(false);
        jTextArea_Vista_Queries.setColumns(20);
        jTextArea_Vista_Queries.setRows(5);
        jScrollPane2.setViewportView(jTextArea_Vista_Queries);

        jTextArea_Resultados.setEditable(false);
        jTextArea_Resultados.setColumns(20);
        jTextArea_Resultados.setRows(5);
        jScrollPane3.setViewportView(jTextArea_Resultados);

        jTextArea_Vista_DB.setEditable(false);
        jTextArea_Vista_DB.setColumns(20);
        jTextArea_Vista_DB.setRows(5);
        jScrollPane4.setViewportView(jTextArea_Vista_DB);

        jLabel1.setText("Vista Previa: ");

        jLabel3.setText("Vista Previa: ");

        jLabel_Vista_Queries.setText("Archivo Queries");

        jLabel_Vista_Queries1.setText("Resultado: ");

        jButton_Exportar_PDF.setText("TXT");

        jLabel2.setText("Exportar resultado a:");

        jButton_Exportar_PDF1.setText(" PDF");

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane4, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel1))
                .addGap(31, 31, 31)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addComponent(jLabel3)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabel_Vista_Queries)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addComponent(jScrollPane3, javax.swing.GroupLayout.PREFERRED_SIZE, 400, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addContainerGap())
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addComponent(jLabel_Vista_Queries1)
                        .addGap(41, 41, 41)
                        .addComponent(jLabel2)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(jButton_Exportar_PDF)
                        .addGap(28, 28, 28)
                        .addComponent(jButton_Exportar_PDF1)
                        .addGap(22, 22, 22))))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createSequentialGroup()
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel1)
                    .addComponent(jLabel3)
                    .addComponent(jLabel_Vista_Queries)
                    .addComponent(jLabel_Vista_Queries1)
                    .addComponent(jLabel2)
                    .addComponent(jButton_Exportar_PDF)
                    .addComponent(jButton_Exportar_PDF1))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane4, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 295, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                        .addComponent(jScrollPane2, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 295, Short.MAX_VALUE)
                        .addComponent(jScrollPane3, javax.swing.GroupLayout.Alignment.LEADING)))
                .addGap(55, 55, 55))
        );

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 61, Short.MAX_VALUE)
        );

        Nucleo.setText("n");

        jLabel4.setText("El S.O detecto");

        jLabel5.setText("nucleos");

        jMenu_File.setText("Archivo");

        JMenu_Nuevo.setText("Nuevo");

        jMenu_Secuencial.setText("Secuencial");
        JMenu_Nuevo.add(jMenu_Secuencial);

        jMenu_Multihilos.setText("Multihilos");
        JMenu_Nuevo.add(jMenu_Multihilos);

        jMenu_Xeon_phi.setText("Xeon Phi");
        JMenu_Nuevo.add(jMenu_Xeon_phi);

        jMenu_GPU.setText("GPU");
        JMenu_Nuevo.add(jMenu_GPU);

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
        jMenuItem_Xenon_Phi.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem_Xenon_PhiActionPerformed(evt);
            }
        });
        JMenu_Nuevo.add(jMenuItem_Xenon_Phi);

        jMenuItem_GPU.setText("GPU");
        jMenuItem_GPU.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItem_GPUActionPerformed(evt);
            }
        });
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
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jLabel4)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(Nucleo)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jLabel5)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jPanel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addComponent(jPanel_Archivos, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jPanel_Archivos, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 18, Short.MAX_VALUE)
                .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, 378, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(62, 62, 62)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(Nucleo)
                            .addComponent(jLabel4)
                            .addComponent(jLabel5)))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(18, 18, 18)
                        .addComponent(jPanel2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    class MenuActionSecuencial implements ActionListener {
        @Override
        public void actionPerformed(ActionEvent e) {
            System.out.println("Selected: " + e.getActionCommand());
            flag_GPU = false;
            flag_Multihilos = false;
            flag_Xenon_Phi = false;
            flag_Secuencial = true;
            jLabel_Hilos.setVisible(false);
            jTextField_Hilos.setVisible(false);
            jButton_Examinar_BD.setVisible(true);
            jButton_Examinar_Queries.setVisible(true);
            jButton_Exportar_PDF.setVisible(true);
            jButton_Procesar_Consultas.setVisible(true);
            jLabel_Archivo_BD.setVisible(true);
            jLabel_Archivo_Queries.setVisible(true);
            jLabel_Hilos.setVisible(false);
            jTextArea_Vista_Queries.setVisible(true);
            jTextField_Archivo_BD.setVisible(true);
            jTextField_Archivo_Queries.setVisible(true);
            jTextField_Hilos.setVisible(false);
            jLabel_Archivo_Queries3.setVisible(true);
            jTextField_Dim.setVisible(true);
            jLabel_TOPK.setVisible(true);
            jTextField_TOPK.setVisible(true);
            jTextArea_Vista_DB.setVisible(true);
            jLabel1.setVisible(true);
            jLabel3.setVisible(true);
            jLabel_Vista_Queries.setVisible(true);
            jTextArea_Resultados.setVisible(true);
            jLabel_Vista_Queries1.setVisible(true);
            jLabel2.setVisible(true);
            jButton_Exportar_PDF1.setVisible(true);
            jTextArea_Resultados.setVisible(true);
        }
    }

    private void jButton_Examinar_BDActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton_Examinar_BDActionPerformed
        JFileChooser explorador = new JFileChooser();
        explorador.setDialogTitle("Seleccionar DB...");
        int seleccion = explorador.showOpenDialog(this);
        File archivo = explorador.getSelectedFile();
        if (seleccion == 0) {
            jTextField_Archivo_BD.setText(archivo.getName());

            char var = 92;
            StringBuffer Ruta = new StringBuffer();
            String caracter = Character.toString(var);
            Ruta_DB = archivo.getPath();
            String[] palabrasSeparadas = Ruta_DB.split(" ");
            for (int i = 0; i < palabrasSeparadas.length; i++) {
                if (i < palabrasSeparadas.length - 1) {
                    Ruta = Ruta.append(palabrasSeparadas[i]).append(" ");
                } else {
                    Ruta = Ruta.append(palabrasSeparadas[i]);
                }
            }
            Ruta_DB = Ruta.toString();

            try {
                tamanho_DB = tamanhoArchivo(archivo.getPath());
            } catch (IOException ex) {
                Logger.getLogger(Interface_Knn.class.getName()).log(Level.SEVERE, null, ex);
            }
            try {
                muestraContenidoDB(archivo.getPath());
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
            char var = 92;
            StringBuffer Ruta = new StringBuffer();
            String caracter = Character.toString(var);
            jTextField_Archivo_Queries.setText(archivo.getName());
            Ruta_Queries = archivo.getPath();
            Ruta_Queries = Ruta_Queries.replaceAll(" ", caracter + " ");
            String[] palabrasSeparadas = Ruta_Queries.split(" ");
            for (int i = 0; i < palabrasSeparadas.length; i++) {
                if (i < palabrasSeparadas.length - 1) {
                    Ruta = Ruta.append(palabrasSeparadas[i] + " ");
                } else {
                    Ruta = Ruta.append(palabrasSeparadas[i]);
                }
            }
            Ruta_Queries = Ruta.toString();
            try {
                tamanho_Queries = tamanhoArchivo(archivo.getPath());
            } catch (IOException ex) {
                Logger.getLogger(Interface_Knn.class.getName()).log(Level.SEVERE, null, ex);
            }
            try {
                muestraContenidoQueries(archivo.getPath());
            } catch (IOException ex) {
                Logger.getLogger(Interface_Knn.class.getName()).log(Level.SEVERE, null, ex);
            }
        } else {

        }
    }//GEN-LAST:event_jButton_Examinar_QueriesActionPerformed

    private void jButton_Procesar_ConsultasActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton_Procesar_ConsultasActionPerformed
        String sSistemaOperativo = System.getProperty("os.name");
        System.out.println(sSistemaOperativo);
        if (sSistemaOperativo.equals("Linux")) {
            if (flag_Secuencial == true) {
                flag_GPU = false;
                flag_Multihilos = false;
                flag_Xenon_Phi = false;
                int i = 0;
                chequear_Secuencial(i);
                if (Ejecutable_Secuencial != null) {
                    try {
                        TOPK = jTextField_TOPK.getText();
                        DIM = jTextField_Dim.getText();
                        String path;
                        path = Ejecutable_Secuencial + " " + Ruta_DB + " " + tamanho_DB + " " + Ruta_Queries + " " + tamanho_Queries + " " + DIM + " " + TOPK;

                        //path = Ejecutable_Secuencial + " " + "\"" + "/home/cristofher/Documentos/Tesis/Knn.git/Codigos/Base de datos/BD_int" + "\"" + " " + 14 + " " + "\"" + "/home/cristofher/Documentos/Tesis/Knn.git/Codigos/Base de datos/BD_int" + "\"" + " " + 14 + " " + 1 + " " + 8;
                        System.out.println(path);
                        //ProcessBuilder pb = new ProcessBuilder(path);
                        //Process p = pb.start();
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
                flag_GPU = false;
                flag_Secuencial = false;
                flag_Xenon_Phi = false;
                int i = 0;
                chequear_Multihilos(i);

                if (Ejecutable_Multihilos != null) {
                    try {
                        TOPK = jTextField_TOPK.getText();
                        DIM = jTextField_Dim.getText();
                        Num_threads = jTextField_Hilos.getText();
                        String path;
                        path = Ejecutable_Multihilos + " " + Ruta_DB + " " + tamanho_DB + " " + Ruta_Queries + " " + tamanho_Queries + " " + " " + Num_threads + " " + TOPK + " " + DIM;
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
            }
            if (flag_Xenon_Phi == true) {
                flag_GPU = false;
                flag_Multihilos = false;
                flag_Secuencial = false;
                int i = 0;
                chequear_MIC(i);
                if (Ejecutable_Xeon_Phi != null) {
                    try {
                        TOPK = jTextField_TOPK.getText();
                        DIM = jTextField_Dim.getText();
                        Num_threads = jTextField_Hilos.getText();
                        String path;
                        path = Ejecutable_Xeon_Phi + " " + Ruta_DB + " " + tamanho_DB + " " + Ruta_Queries + " " + tamanho_Queries + " " + DIM + " " + TOPK;
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
            }
            if (flag_GPU == true) {
                flag_Xenon_Phi = false;
                flag_Multihilos = false;
                flag_Secuencial = false;
                int i = 0;
                chequear_GPU(i);
                if (Ejecutable_GPU != null) {
                    try {
                        TOPK = jTextField_TOPK.getText();
                        DIM = jTextField_Dim.getText();
                        Num_threads = jTextField_Hilos.getText();
                        String path;
                        path = Ejecutable_Xeon_Phi + " " + Ruta_DB + " " + Ruta_Queries + " " + tamanho_DB + " " + tamanho_Queries + " " + DIM + " " + TOPK;
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
            }

        }
    }//GEN-LAST:event_jButton_Procesar_ConsultasActionPerformed

    private void jMenuItem_SecuencialActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem_SecuencialActionPerformed

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
        jButton_Procesar_Consultas.setVisible(true);
        jLabel_Archivo_BD.setVisible(true);
        jLabel_Archivo_Queries.setVisible(true);
        jLabel_Hilos.setVisible(true);
        jTextArea_Vista_Queries.setVisible(true);
        jLabel_Archivo_Queries3.setVisible(true);
        jTextField_Dim.setVisible(true);
        jLabel_TOPK.setVisible(true);
        jTextField_TOPK.setVisible(true);
        jTextArea_Vista_DB.setVisible(true);
        jLabel1.setVisible(true);
        jLabel3.setVisible(true);
        jLabel_Vista_Queries.setVisible(true);
        jTextArea_Resultados.setVisible(true);
        jLabel_Vista_Queries1.setVisible(true);
        jLabel2.setVisible(true);
        jButton_Exportar_PDF1.setVisible(true);
        jTextArea_Resultados.setVisible(true);
    }//GEN-LAST:event_jMenuItem_MultihilosActionPerformed

    private void jTextField_DimKeyTyped(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_jTextField_DimKeyTyped
        char c = evt.getKeyChar();
        if (Character.isLetter(c) || Character.isSpaceChar(c)) {
            getToolkit().beep();

            evt.consume();
            Object msj = "Solo ingrese Números";
            JOptionPane.showMessageDialog(null,
                    msj,
                    "Advertencia",
                    JOptionPane.WARNING_MESSAGE);
        }
    }//GEN-LAST:event_jTextField_DimKeyTyped

    private void jTextField_TOPKKeyTyped(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_jTextField_TOPKKeyTyped
        char c = evt.getKeyChar();
        if (Character.isLetter(c) || Character.isSpaceChar(c)) {
            getToolkit().beep();

            evt.consume();
            Object msj = "Solo ingrese Números";
            JOptionPane.showMessageDialog(null,
                    msj,
                    "Advertencia",
                    JOptionPane.WARNING_MESSAGE);
        }
    }//GEN-LAST:event_jTextField_TOPKKeyTyped

    private void jTextField_HilosKeyTyped(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_jTextField_HilosKeyTyped
        char c = evt.getKeyChar();
        if (Character.isLetter(c) || Character.isSpaceChar(c)) {
            getToolkit().beep();
            evt.consume();
            Object msj = "Solo ingrese Números";
            JOptionPane.showMessageDialog(null,
                    msj,
                    "Advertencia",
                    JOptionPane.WARNING_MESSAGE);
        }
    }//GEN-LAST:event_jTextField_HilosKeyTyped

    private void jMenuItem_Xenon_PhiActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem_Xenon_PhiActionPerformed
        flag_GPU = false;
        flag_Multihilos = false;
        flag_Xenon_Phi = true;
        flag_Secuencial = false;
        jLabel_Hilos.setVisible(false);
        jTextField_Archivo_BD.setVisible(true);
        jTextField_Archivo_Queries.setVisible(true);
        jTextField_Hilos.setVisible(false);
        jButton_Examinar_BD.setVisible(true);
        jButton_Examinar_Queries.setVisible(true);
        jButton_Exportar_PDF.setVisible(true);
        jButton_Procesar_Consultas.setVisible(true);
        jLabel_Archivo_BD.setVisible(true);
        jLabel_Archivo_Queries.setVisible(true);
        jLabel_Hilos.setVisible(false);
        jTextArea_Vista_Queries.setVisible(true);
        jLabel_Archivo_Queries3.setVisible(true);
        jTextField_Dim.setVisible(true);
        jLabel_TOPK.setVisible(true);
        jTextField_TOPK.setVisible(true);
        jTextArea_Vista_DB.setVisible(true);
        jLabel1.setVisible(true);
        jLabel3.setVisible(true);
        jLabel_Vista_Queries.setVisible(true);
        jTextArea_Resultados.setVisible(true);
        jLabel_Vista_Queries1.setVisible(true);
        jLabel2.setVisible(true);
        jButton_Exportar_PDF1.setVisible(true);
        jTextArea_Resultados.setVisible(true);
    }//GEN-LAST:event_jMenuItem_Xenon_PhiActionPerformed

    private void jMenuItem_GPUActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItem_GPUActionPerformed
        flag_GPU = true;
        flag_Multihilos = false;
        flag_Xenon_Phi = false;
        flag_Secuencial = false;
        jLabel_Hilos.setVisible(false);
        jTextField_Archivo_BD.setVisible(true);
        jTextField_Archivo_Queries.setVisible(true);
        jTextField_Hilos.setVisible(false);
        jButton_Examinar_BD.setVisible(true);
        jButton_Examinar_Queries.setVisible(true);
        jButton_Exportar_PDF.setVisible(true);
        jButton_Procesar_Consultas.setVisible(true);
        jLabel_Archivo_BD.setVisible(true);
        jLabel_Archivo_Queries.setVisible(true);
        jLabel_Hilos.setVisible(false);
        jTextArea_Vista_Queries.setVisible(true);
        jLabel_Archivo_Queries3.setVisible(true);
        jTextField_Dim.setVisible(true);
        jLabel_TOPK.setVisible(true);
        jTextField_TOPK.setVisible(true);
        jTextArea_Vista_DB.setVisible(true);
        jLabel1.setVisible(true);
        jLabel3.setVisible(true);
        jLabel_Vista_Queries.setVisible(true);
        jTextArea_Resultados.setVisible(true);
        jLabel_Vista_Queries1.setVisible(true);
        jLabel2.setVisible(true);
        jButton_Exportar_PDF1.setVisible(true);
        jTextArea_Resultados.setVisible(true);
    }//GEN-LAST:event_jMenuItem_GPUActionPerformed

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

    @SuppressWarnings("empty-statement")
    int tamanhoArchivo(String archivo) throws FileNotFoundException, IOException {
        int linea = 0;
        FileReader f = new FileReader(archivo);
        try (BufferedReader b = new BufferedReader(f)) {
            while ((b.readLine()) != null) {;
                linea++;
            }
        }
        return linea;
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

    void chequear_Secuencial(int i) {
        Object msj = "Faltan los siquientes argumentos:";
        String nl = System.getProperty("line.separator");
        if ("".equals(jTextField_Archivo_BD.getText()) || "".equals(jTextField_Archivo_Queries.getText())
                || "".equals(jTextField_Dim.getText()) || "".equals(jTextField_TOPK.getText())) {
            if ("".equals(jTextField_Archivo_BD.getText())) {
                msj = msj + nl + "- Archivo BD";
            }
            if ("".equals(jTextField_Archivo_Queries.getText())) {
                msj = msj + nl + "- Archivo Queries ";
            }
            if ("".equals(jTextField_Dim.getText())) {
                msj = msj + nl + "- Dimension del objeto";
            }
            if ("".equals(jTextField_TOPK.getText())) {
                msj = msj + nl + "- TOPK";
            }
            JOptionPane.showMessageDialog(null,
                    msj,
                    "Mensaje de Error",
                    JOptionPane.ERROR_MESSAGE
            );
            Ejecutable_GPU = null;
            Ejecutable_Multihilos = null;
            Ejecutable_Secuencial = null;
            Ejecutable_Xeon_Phi = null;
        } else {
            Ejecutable_Secuencial = rutas_Secuenciales[1];
        }
    }

    void chequear_Multihilos(int i) {
        Object msj = "Faltan los siquientes argumentos:";
        String nl = System.getProperty("line.separator");
        if ("".equals(jTextField_Archivo_BD.getText()) || "".equals(jTextField_Archivo_Queries.getText())
                || "".equals(jTextField_Dim.getText()) || "".equals(jTextField_TOPK.getText()) || "".equals(jTextField_Hilos.getText())) {
            if ("".equals(jTextField_Archivo_BD.getText())) {
                msj = msj + nl + "- Archivo BD";
            }
            if ("".equals(jTextField_Archivo_Queries.getText())) {
                msj = msj + nl + "- Archivo Queries ";
            }
            if ("".equals(jTextField_Dim.getText())) {
                msj = msj + nl + "- Dimension del objeto";
            }
            if ("".equals(jTextField_TOPK.getText())) {
                msj = msj + nl + "- TOPK";
            }
            if ("".equals(jTextField_Hilos.getText())) {
                msj = msj + nl + "- Número de hilos";
            }
            JOptionPane.showMessageDialog(null,
                    msj,
                    "Mensaje de Error",
                    JOptionPane.ERROR_MESSAGE
            );
            Ejecutable_GPU = null;
            Ejecutable_Multihilos = null;
            Ejecutable_Secuencial = null;
            Ejecutable_Xeon_Phi = null;
        } else {
            Ejecutable_Multihilos = rutas_Multihilos[2];
        }
    }

    void chequear_MIC(int i) {
        Object msj = "Faltan los siquientes argumentos:";
        String nl = System.getProperty("line.separator");
        if ("".equals(jTextField_Archivo_BD.getText()) || "".equals(jTextField_Archivo_Queries.getText())
                || "".equals(jTextField_Dim.getText()) || "".equals(jTextField_TOPK.getText()) || "".equals(jTextField_Hilos.getText())) {
            if ("".equals(jTextField_Archivo_BD.getText())) {
                msj = msj + nl + "- Archivo BD";
            }
            if ("".equals(jTextField_Archivo_Queries.getText())) {
                msj = msj + nl + "- Archivo Queries ";
            }
            if ("".equals(jTextField_Dim.getText())) {
                msj = msj + nl + "- Dimension del objeto";
            }
            if ("".equals(jTextField_TOPK.getText())) {
                msj = msj + nl + "- TOPK";
            }
            if ("".equals(jTextField_Hilos.getText())) {
                msj = msj + nl + "- Número de hilos";
            }
            JOptionPane.showMessageDialog(null,
                    msj,
                    "Mensaje de Error",
                    JOptionPane.ERROR_MESSAGE
            );
            Ejecutable_GPU = null;
            Ejecutable_Multihilos = null;
            Ejecutable_Secuencial = null;
            Ejecutable_Xeon_Phi = null;
        } else {
            Ejecutable_Xeon_Phi = rutas_Xeon_phi[3];
            System.out.println(rutas_Xeon_phi[3]);
        }
    }

    void chequear_GPU(int i) {
        Object msj = "Faltan los siquientes argumentos:";
        String nl = System.getProperty("line.separator");
        if ("".equals(jTextField_Archivo_BD.getText()) || "".equals(jTextField_Archivo_Queries.getText())
                || "".equals(jTextField_Dim.getText()) || "".equals(jTextField_TOPK.getText()) || "".equals(jTextField_Hilos.getText())) {
            if ("".equals(jTextField_Archivo_BD.getText())) {
                msj = msj + nl + "- Archivo BD";
            }
            if ("".equals(jTextField_Archivo_Queries.getText())) {
                msj = msj + nl + "- Archivo Queries ";
            }
            if ("".equals(jTextField_Dim.getText())) {
                msj = msj + nl + "- Dimension del objeto";
            }
            if ("".equals(jTextField_TOPK.getText())) {
                msj = msj + nl + "- TOPK";
            }
            if ("".equals(jTextField_Hilos.getText())) {
                msj = msj + nl + "- Número de hilos";
            }
            JOptionPane.showMessageDialog(null,
                    msj,
                    "Mensaje de Error",
                    JOptionPane.ERROR_MESSAGE
            );
            Ejecutable_GPU = null;
            Ejecutable_Multihilos = null;
            Ejecutable_Secuencial = null;
            Ejecutable_Xeon_Phi = null;
        } else {
            Ejecutable_GPU = rutas_GPU[i];
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
            java.util.logging.Logger.getLogger(Interface_Knn.class
                    .getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }

        java.awt.EventQueue.invokeLater(() -> {
            try {
                new Interface_Knn().setVisible(true);
            } catch (IOException ex) {
                Logger.getLogger(Interface_Knn.class.getName()).log(Level.SEVERE, null, ex);
            }
        });
    }

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JMenu JMenu_Nuevo;
    private javax.swing.JMenuBar Menu_Principal;
    private javax.swing.JLabel Nucleo;
    public static javax.swing.ButtonGroup Tipo;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton_Examinar_BD;
    private javax.swing.JButton jButton_Examinar_Queries;
    private javax.swing.JButton jButton_Exportar_PDF;
    private javax.swing.JButton jButton_Exportar_PDF1;
    private javax.swing.JButton jButton_Procesar_Consultas;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel_Archivo_BD;
    private javax.swing.JLabel jLabel_Archivo_Queries;
    private javax.swing.JLabel jLabel_Archivo_Queries3;
    private javax.swing.JLabel jLabel_Hilos;
    private javax.swing.JLabel jLabel_TOPK;
    private javax.swing.JLabel jLabel_Vista_Queries;
    private javax.swing.JLabel jLabel_Vista_Queries1;
    private javax.swing.JMenuItem jMenuItem_GPU;
    private javax.swing.JMenuItem jMenuItem_Multihilos;
    private javax.swing.JMenuItem jMenuItem_Salir;
    private javax.swing.JMenuItem jMenuItem_Secuencial;
    private javax.swing.JMenuItem jMenuItem_Xenon_Phi;
    private javax.swing.JMenu jMenu_Edit;
    private javax.swing.JMenu jMenu_File;
    private javax.swing.JMenu jMenu_GPU;
    private javax.swing.JMenu jMenu_Multihilos;
    private javax.swing.JMenu jMenu_Secuencial;
    private javax.swing.JMenu jMenu_Xeon_phi;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel_Archivos;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JScrollPane jScrollPane3;
    private javax.swing.JScrollPane jScrollPane4;
    private javax.swing.JSeparator jSeparator1;
    private javax.swing.JTextArea jTextArea_Resultados;
    private javax.swing.JTextArea jTextArea_Vista_DB;
    private javax.swing.JTextArea jTextArea_Vista_Queries;
    private javax.swing.JTextField jTextField_Archivo_BD;
    private javax.swing.JTextField jTextField_Archivo_Queries;
    private javax.swing.JTextField jTextField_Dim;
    private javax.swing.JTextField jTextField_Hilos;
    private javax.swing.JTextField jTextField_TOPK;
    // End of variables declaration//GEN-END:variables

}
