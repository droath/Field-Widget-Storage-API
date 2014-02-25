<?php

/**
 * @file
 * The base class for the field widget SQL storage.
 */

class FieldWidgetSqlStorageDatabase {

  protected $table;
  protected $records;

  /**
   * The field widget SQL storage database constructor.
   *
   * @param $table
   *   The table name.
   * @param $records
   *   The database table records.
   */
  public function __construct($table, $records) {
    $this->table = $table;
    $this->records = $records;
  }

  /**
   * Save the database table records.
   *
   * @return int
   *   A integer will be return based on if the records were saved or updated.
   */
  public function save() {
    $records = $this->records;

    // Serialize the fields record if it has set the serialized flag.
    foreach ($this->getFields() as $field_name => $info) {
      if (isset($records[$field_name]) &&
        (isset($info['serialized']) && $info['serialized'])) {
          $records[$field_name] = serialize($records[$field_name]);
      }
    }

    // Save or update the provided records for the given table.
    $op = db_merge($this->table)
      ->key($this->getPrimaryKeys())
      ->fields($records)
      ->execute();

    return $op;
  }

  /**
   * Delete the database table records.
   */
  public function delete() {
    $query = db_delete($this->table);

    foreach ($this->records as $field_name => $value) {
      $query->condition($field_name, $value);
    }

    return $query->execute();
  }

  /**
   * Build the query for retrieving the table records.
   *
   * @param $conditions
   *   An array of conditions that need to be checked.
   *
   * @return object
   *   An SelectQuery object.
   */
  protected function retrieveQuery($conditions) {
    $query = db_select($this->table, 'tb')
      ->fields('tb', $this->records);

    // Iterate through the query conditions if any exist.
    foreach ($conditions as $field_name => $value) {
      $query->condition($field_name, $value);
    }

    return $query->execute();
  }

  /**
   * Retrieve a single record from the database table.
   *
   * @param $conditions
   *   An array of conditions
   * @param $fetch_style
   *   A PDO fetch style constants.
   *
   * @return array|object
   *   An array or object can be return which is based off your fetch style.
   */
  public function retrieve($conditions, $fetch_style) {
    $fields  = $this->getFields();
    $results = $this->retrieveQuery($conditions)->fetch($fetch_style);

    // Check if any of the fields need to be unserialized.
    foreach ($results as $field_name => &$value) {
      if (isset($fields[$field_name]['serialized']) && $fields[$field_name]['serialized']) {
        $value = unserialize($value);
      }
    }

    return $results;
  }

  /**
   * Retrieve all records from the database table.
   *
   * @param $conditions
   *   An array of conditions
   * @param $fetch_style
   *   A PDO fetch style constants.
   *
   * @return array|object
   *   An array or object can be return which is based off your fetch style.
   */
  public function retrieveAll($conditions, $column_index, $fetch_style) {
    $fields = $this->getFields();
    $query  = $this->retrieveQuery($conditions);

    $results = (isset($column_index) && !is_null($column_index)) ? $query->fetchAllAssoc($column_index, $fetch_style) : $query->fetchAll($fetch_style);

    // Check if any of the fields need to be unserialized.
    foreach ($results as &$info) {
      foreach ($info as $field_name => &$value) {
        if (isset($fields[$field_name]['serialized']) && $fields[$field_name]['serialized']) {
          $value = unserialize($value);
        }
      }
    }

    return $results;
  }

  /**
   * Check if the records exist.
   *
   * @param $conditions
   *   An array of conditions.
   *
   * @return bool
   *   Either true or false depending if the records exist in the database.
   */
  public function hasData($conditions) {
    return $this->retrieveQuery($conditions)->rowCount() ? TRUE : FALSE;
  }

  /**
   * Get the database table schema.
   *
   * @return array
   *   An array of the schema.
   */
  public function getSchema() {
    return drupal_get_schema($this->table);
  }

  /**
   * Get the database table fields from the schema.
   *
   * @return array
   *   An array of the table fields.
   */
  public function getFields() {
    $schema = $this->getSchema();
    return $schema['fields'];
  }

  /**
   * Get the database table primary keys from the schema.
   *
   * @return array
   *   An array of the table primary keys.
   */
  public function getPrimaryKeys() {
    $schema = $this->getSchema();
    return array_intersect_key($this->records, array_flip($schema['primary key']));
  }
}
