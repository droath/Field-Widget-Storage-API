<?php

/**
 * @file
 * The base class for retrieving entity information.
 */

class FieldWidgetSqlStorageEntity {

  protected $entity;
  protected $bundle;
  protected $entityId;
  protected $revisionId;
  protected $entityType;

  /**
   * The field widget SQL storage entity constructor.
   *
   * @param $entity
   *   An entity object.
   * @param $entity_type
   *   The type of entity.
   */
  public function __construct($entity, $entity_type) {
    $this->entity = $entity;
    $this->entityType = $entity_type;

    // Extract the entity ids (id, rid, bundle) from the entity object.
    list($this->entityId, $this->revisionId, $this->bundle) = entity_extract_ids($this->entityType, $this->entity);
  }

  /**
   * Get the entity field instances.
   *
   * @return array
   *   If $entity_type is not set, return all instances keyed by entity type
   *   and bundle name. If $entity_type is set, return all instances for that
   *   entity type, keyed by bundle name. If $entity_type and $bundle_name are
   *   set, return all instances for that bundle.
   */
  public function getInstnaces() {
    return field_info_instances($this->entityType, $this->bundle);
  }

  /**
   * Get the entity field widget SQL storage information.
   *
   * @return array
   *   An array of the instance storage info.
   */
  public function getFieldsWidgetStorage() {
    $storage = array();

    foreach ($this->getInstnaces() as $field_name => $instance) {
      $widget = $instance['widget'];
      if ($widget_schema = field_widget_sql_storage_get_widget_schema_by_module($widget['module'])) {
        $storage[$field_name] = $widget_schema;
      }
    }

    return $storage;
  }

  /**
   * Get the entity object.
   *
   * @return object
   *   An entity object.
   */
  public function getEntity() {
    return $this->entity;
  }

  /**
   * Get the entity bundle type.
   *
   * @return string
   *   The entity bundle type.
   */
  public function getBundle() {
    return $this->bundle;
  }

  /**
   * Get the entity ID.
   *
   * @return string
   *   The entity ID.
   */
  public function getEntityId() {
    return $this->entityId;
  }

  /**
   * Get the entity revision ID.
   *
   * @return string
   *   The entity revision ID.
   */
  public function getRevisionId() {
    // If the entity doesn't support revisions; then set revision id to the
    // entity id.
    return isset($this->revisionId) ? $this->revisionId : $this->getEntityId();
  }

  /**
   * Get the entity type.
   *
   * @return string
   *   The type of entity.
   */
  public function getEntityType() {
    return $this->entityType;
  }
}
