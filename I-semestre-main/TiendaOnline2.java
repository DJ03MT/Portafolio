package com.mycompany.guia1;

import java.io.*;
import java.util.ArrayList;
import java.util.Date;
import java.util.Scanner;

class Producto implements Serializable {

    private static final long serialVersionUID = 1L;

    private String nombre;
    private double precio;
    private int cantidad;

    public Producto(String nombre, double precio, int cantidad) {
        this.nombre = nombre;
        this.precio = precio;
        this.cantidad = cantidad;
    }

    public String getNombre() {
        return nombre;
    }

    public void setNombre(String nombre) {
        this.nombre = nombre;
    }

    public double getPrecio() {
        return precio;
    }

    public void setPrecio(double precio) {
        this.precio = precio;
    }

    public int getCantidad() {
        return cantidad;
    }

    public void setCantidad(int cantidad) {
        this.cantidad = cantidad;
    }
}

class Factura implements Serializable {

    private static final long serialVersionUID = 1L;

    private String nombreCliente;
    private ArrayList<Producto> productos;
    private Date fecha;

    public Factura(String nombreCliente, ArrayList<Producto> productos) {
        this.nombreCliente = nombreCliente;
        this.productos = productos;
        this.fecha = new Date();
    }

    public String getNombreCliente() {
        return nombreCliente;
    }

    public void setNombreCliente(String nombreCliente) {
        this.nombreCliente = nombreCliente;
    }

    public ArrayList<Producto> getProductos() {
        return productos;
    }

    public void setProductos(ArrayList<Producto> productos) {
        this.productos = productos;
    }

    public Date getFecha() {
        return fecha;
    }

    public void setFecha(Date fecha) {
        this.fecha = fecha;
    }

    public double calcularTotal() {
        double total = 0;
        for (Producto producto : productos) {
            total += producto.getPrecio() * producto.getCantidad();
        }
        return total;
    }
}

class Inventario implements Serializable {

    private static final long serialVersionUID = 1L;

    private ArrayList<Producto> productos;

    public Inventario() {
        this.productos = new ArrayList<>();
    }

    public void agregarProducto(Producto producto) {
        productos.add(producto);
    }

    public void quitarProducto(Producto producto) {
        productos.remove(producto);
    }

    public void verInventario() {
        if (productos.isEmpty()) {
            System.out.println("El inventario está vacío.");
        } else {
            System.out.println("Inventario:");
            for (int i = 0; i < productos.size(); i++) {
                Producto producto = productos.get(i);
                System.out.println(i + ". " + producto.getNombre() + " - Precio: " + producto.getPrecio() + " - Cantidad: " + producto.getCantidad());
            }
        }
    }

    public ArrayList<Producto> getProductos() {
        return productos;
    }

    public void guardarInventario() {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("inventario.dat"))) {
            oos.writeObject(this);
            System.out.println("Inventario guardado correctamente.");
        } catch (IOException e) {
            System.out.println("Error al guardar el inventario.");
            e.printStackTrace();
        }
    }

    public void cargarInventario() {
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream("inventario.dat"))) {
            productos = (ArrayList<Producto>) ois.readObject();
            System.out.println("Inventario cargado correctamente.");
        } catch (IOException | ClassNotFoundException e) {
            System.out.println("Error al cargar el inventario.");
            e.printStackTrace();
        }
    }
}

public class TiendaOnline2 {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        Inventario inventario = new Inventario();

        while (true) {
            System.out.println("Menú Principal:");
            System.out.println("1. Pedido");
            System.out.println("2. Inventario");
            System.out.println("3. Salir");

            int opcion;
            while (true) {
                if (scanner.hasNextInt()) {
                    opcion = scanner.nextInt();
                    scanner.nextLine();
                    break;
                } else {
                    System.out.println("Opción no válida. Inténtalo de nuevo.");
                    scanner.nextLine();
                }
            }

            switch (opcion) {
                case 1:
                    gestionarPedido(inventario, scanner);
                    break;
                case 2:
                    gestionarInventario(inventario, scanner);
                    break;
                case 3:
                    System.out.println("¡Gracias por usar la tienda online!");
                    System.exit(0);
                    break;
                default:
                    System.out.println("Opción no válida. Inténtalo de nuevo.");
            }
        }
    }

    private static void gestionarPedido(Inventario inventario, Scanner scanner) {
        ArrayList<Producto> productosPedido = new ArrayList<>();

        while (true) {
            System.out.println("Menú Pedido:");
            System.out.println("1. Tienda");
            System.out.println("2. Confirmar Pedido");
            System.out.println("3. Cancelar Pedido");

            int opcion = scanner.nextInt();
            scanner.nextLine();

            switch (opcion) {
                case 1:
                    agregarProducto(inventario, productosPedido, "Ropa", scanner);
                    break;
                case 2:
                    generarFactura(productosPedido);
                    return;
                case 3:
                    System.out.println("Pedido cancelado.");
                    return;
                default:
                    System.out.println("Opción no válida. Inténtalo de nuevo.");
            }
        }
    }

    private static void agregarProducto(Inventario inventario, ArrayList<Producto> productosPedido, String categoria, Scanner scanner) {
        inventario.verInventario();
        System.out.println("Seleccione un producto de " + categoria + " (Ingrese el número):");

        int indiceProducto = -1;
        while (true) {
            if (scanner.hasNextInt()) {
                indiceProducto = scanner.nextInt();
                scanner.nextLine();

                if (indiceProducto >= 0 && indiceProducto < inventario.getProductos().size()) {
                    break;
                } else {
                    System.out.println("Número de producto no válido. Inténtalo de nuevo.");
                }
            } else {
                System.out.println("Entrada no válida. Inténtalo de nuevo.");
                scanner.nextLine();
            }
        }

        Producto productoSeleccionado = inventario.getProductos().get(indiceProducto);

        System.out.println("Ingrese la cantidad:");
        while (!scanner.hasNextInt()) {
            System.out.println("Cantidad inválida. Ingrese un número entero:");
            scanner.next();
        }
        int cantidad = scanner.nextInt();
        scanner.nextLine();

        if (cantidad > 0 && cantidad <= productoSeleccionado.getCantidad()) {
            Producto productoEnPedido = new Producto(productoSeleccionado.getNombre(), productoSeleccionado.getPrecio(), cantidad);
            productosPedido.add(productoEnPedido);
            System.out.println("Producto agregado al pedido.");
        } else {
            System.out.println("Cantidad no válida. Inténtalo de nuevo.");
        }
    }

    private static void generarFactura(ArrayList<Producto> productosPedido) {

        Scanner scanner = new Scanner(System.in);
        System.out.println("Ingrese su nombre:");
        String nombreCliente = scanner.nextLine();
        Factura factura = new Factura(nombreCliente, new ArrayList<>(productosPedido));

        System.out.println("\nDetalle de la Factura:");
        for (Producto producto : productosPedido) {
            System.out.println(producto.getNombre() + " - Precio: " + producto.getPrecio() + " - Cantidad: " + producto.getCantidad());
        }

        System.out.println("Total: " + factura.calcularTotal());

        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("factura_" + System.currentTimeMillis() + ".dat"))) {
            oos.writeObject(factura);
            System.out.println("Factura guardada correctamente.");
        } catch (IOException e) {
            System.out.println("Error al guardar la factura.");
            e.printStackTrace();
        }

        try {
            Thread.sleep(3000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println("¡Compra realizada con éxito!");
    }

    private static void gestionarInventario(Inventario inventario, Scanner scanner) {
        while (true) {
            System.out.println("Menú Inventario:");
            System.out.println("1. Agregar Producto");
            System.out.println("2. Quitar Producto");
            System.out.println("3. Volver al Menú Principal");

            int opcion;
            while (true) {
                if (scanner.hasNextInt()) {
                    opcion = scanner.nextInt();
                    scanner.nextLine();
                    break;
                } else {
                    System.out.println("Opción no válida. Inténtalo de nuevo.");
                    scanner.nextLine();
                }
            }

            switch (opcion) {
                case 1:
                    agregarProductoInventario(inventario, scanner);
                    break;

                case 2:
                    quitarProductoInventario(inventario, scanner);
                    break;

                case 3:
                    return;
                default:
                    System.out.println("Opción no válida. Inténtalo de nuevo.");
            }
        }
    }

    private static void agregarProductoInventario(Inventario inventario, Scanner scanner) {
        System.out.println("Ingrese el nombre del nuevo producto:");
        String nombre = scanner.nextLine();
        System.out.println("Ingrese el precio del nuevo producto:");
        while (!scanner.hasNextDouble()) {
            System.out.println("Precio inválido. Ingrese un número:");
            scanner.next();
        }
        double precio = scanner.nextDouble();
        System.out.println("Ingrese la cantidad del nuevo producto:");
        while (!scanner.hasNextInt()) {
            System.out.println("Cantidad inválida. Ingrese un número entero:");
            scanner.next();
        }
        int cantidad = scanner.nextInt();
        scanner.nextLine();

        Producto nuevoProducto = new Producto(nombre, precio, cantidad);
        inventario.agregarProducto(nuevoProducto);

        System.out.println("Producto agregado al inventario.");
    }

    private static void quitarProductoInventario(Inventario inventario, Scanner scanner) {
        inventario.verInventario();
        System.out.println("Ingrese el número del producto que desea quitar:");
        int numeroProducto = -1;
        while (true) {
            if (scanner.hasNextInt()) {
                numeroProducto = scanner.nextInt();
                scanner.nextLine();

                if (numeroProducto >= 0 && numeroProducto < inventario.getProductos().size()) {
                    break;
                } else {
                    System.out.println("Número de producto no válido. Inténtalo de nuevo.");
                }
            } else {
                System.out.println("Entrada no válida. Inténtalo de nuevo.");
                scanner.nextLine();
            }
        }

        Producto productoSeleccionado = inventario.getProductos().get(numeroProducto);

        inventario.quitarProducto(productoSeleccionado);
        System.out.println("Producto quitado del inventario.");
    }
}
